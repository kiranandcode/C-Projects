#include <stdio.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdlib.h>

#define PNG_SIGNATURE {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A}

struct chunk {
    char length[4];
    char type[4];
    char *data;
    char CRC[4];
};

struct png {
    int chunk_count;
    struct chunk *chunks;
};

uint8_t fsize(FILE *fp) {
    int prev = ftell(fp); // Store the current position
    fseek(fp, 0L, SEEK_END);
    int sz = ftell(fp) - prev;
    fseek(fp, prev, SEEK_SET);
    return sz;
}

void printHexidecimal(FILE *fp, uint8_t *data, size_t bytes) {
    int i;
    for(i = 0; i<bytes; i++) {
        fprintf(fp, "0x%x ", data[i]);
    }
}

uint8_t byte_array_to_int(char *byte_array, size_t bytes) {
    uint8_t size = 0;
    int i;
    for(i = 0; i<bytes; i++) {
        size |=  (byte_array[i] << (8*(bytes-i-1)));
    }

    return size;
}

struct png *getPng(char *file_name) {
    FILE *fp;
    fp = fopen(file_name, "rb");

    // First check the signature
    uint8_t head[8];
    uint8_t signature[8] = PNG_SIGNATURE;
    fread(head, sizeof(head), 1, fp);

    if(memcmp(signature, head, 8)) {
        fprintf(stderr, "Filename provided did not point to a PNG file\n");
        fclose(fp);
        return NULL;
    }

    char length[4];
    char type[4];
    char *data;
    char CRC[4];

    uint8_t chunk_count = 0;
    struct chunk *chunks = NULL;

    while(fread(length, sizeof(length),1, fp)) {
        if(!fread(type, sizeof(type),1, fp)){
            fprintf(stderr, "Error in png file - 1st check");
            fclose(fp);
            return NULL;
        }
        uint8_t size =byte_array_to_int(length,4);
        fprintf(stderr, "Length of data in Chunk: %d\n", size);

        data = malloc(sizeof(char) * size);
        int data_read;
        if((((data_read = fread(data, (sizeof(char) * size), 1, fp)) == 0) && size != 0)) {
            fprintf(stderr, "Error in png file - 2nd check, trying to read %d bytes, read %d", size, data_read);
            //fclose(fp);
            //return NULL;
        }
        if(!fread(CRC, sizeof(CRC), 1, fp)) {
            fprintf(stderr, "Error in png file - 3rd check");
            //fclose(fp);
            //return NULL;

        }
        chunk_count++;
        chunks = realloc(chunks, sizeof(struct chunk)*chunk_count);
        struct chunk *new = &chunks[chunk_count-1];

        int i;
        for(i = 0; i<4; i++) {
            new->length[i] = length[i];
            new->type[i] = type[i];
            new->CRC[i] = CRC[i];
        }
        new->data = data;
    }


    fclose(fp);
    struct png *image = malloc(sizeof(struct png));
    image->chunk_count = chunk_count;
    image->chunks = chunks;
    return image;
}

void writePngToFile(FILE *fp, struct png *image) {
    int i;
    uint8_t signature[8] = PNG_SIGNATURE;

    fwrite(signature, sizeof(signature), 1, fp);
    int s = image->chunk_count;
    printf("Printing %d chunks\n", s);
    for(i = 0; i<image->chunk_count; i++) {
        struct chunk c = image->chunks[i];
        fwrite(c.length, sizeof(c.length),1, fp);
        fwrite(c.type, sizeof(c.type), 1, fp);
        uint8_t size = byte_array_to_int(c.length, 4);
        fwrite(c.data, sizeof(char)*size, 1, fp);
        fwrite(c.CRC, sizeof(c.CRC),1, fp);
    }
}

struct rec {
    int x, y, z;

};

int main() {

    int counter;
    FILE *fp;
    struct rec record;
    fp = fopen("test.bin", "rb");
    if(!fp) {
        fprintf(stderr, "Could not open file");
        return 1;
    }
    /*
    for(counter = 1; counter <= 10; counter ++) {

        record.x = counter;
        fwrite(&record, sizeof(struct rec), 1, fp);
        fread(&record, sizeof(struct rec), 1,fp);
        printf("%d\n", record.x);
    }*/


    fclose(fp);
    int a = byte_array_to_int((char[]){0x00, 0x00, 0x00, 0x0D},4);

    printf("%d\n", a);
    struct png *image = getPng("test.png");
    FILE *new = fopen("new.png", "wb");
    writePngToFile(new, image);
    fclose(new);

    return 0;
}