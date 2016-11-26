#include <stdio.h>
#include <stdlib.h>                 /* malloc, free and rand */
#include <string.h>                 /* String functions */
const int Fsize = 50;
const int cases = 6;



void parseInput(void);
int parse (char *g);

typedef struct node {
    char operator;
    struct node *leftChild;
    struct node *rightChild;
} Node;

void parseForm(char *start, Node **root);
void printParseTree(Node *root);

int main()
{
    char inp[] = "((E[xy]^E[xz])^(E[xy]^E[zy]))";
    fprintf(stdout, "%d\n", strlen(inp));
    Node *root;
    parseForm(inp, &root);
    printParseTree(root);


    return 0;
}


void parseInput(void) {
    char *name = malloc(Fsize);
    FILE *fp, *fpout;

    /*reads from input.txt, writes to output.txt*/
    if( (fp = fopen("input.txt", "r")) == NULL ) fprintf(stderr, "ERROR: Could not open input.txt - does it exist?\n"), exit(1);
    if( (fpout = fopen("output.txt", "w")) == NULL ) fprintf(stderr, "ERROR: Could not open output.txt - is it read-only?\n"), exit(1);

    int j, parseResult;

    for(j = 0; j < cases; j++) {
        fscanf(fp, "%s", name);
        parseResult=parse(name);

        switch(parseResult) {
        case 1:
            fprintf(fpout, "%s is an atomic formula. \n", name);
            break;
        case 2:
            fprintf(fpout, "%s is a negated formula. \n", name);
            break;
        case 3:
            fprintf(fpout, "%s is a binary connective formula. \n", name);
            break;
        case 4:
            fprintf(fpout, "%s is an existential formula. \n", name);
            break;
        case 5:
            fprintf(fpout, "%s is a universal formula. \n", name);
            break;
        default:
            fprintf(fpout, "%s is not a formula. \n", name);
            break;
        }
    }
}



int parse(char *g)
{
    /*return 1 if atomic, return 2 if negation, return 3 if binary, 4 is exists, 5 if for all, ow 0 */
        switch(*g) {
        case '(': break;
        case ')': break;
        case 'v':
        case '^':
        case '>': break;
        case 'X': break;
        case '-': break;
        case 'E': break;
        case 'A': break;
        default:  break;
        }
    return 0;
}

void parseForm(char *start, Node **root) {
    *root = malloc(sizeof(Node));
    char *ref = start;
    int scope = 0;
    char op = '\0';
    char *second = 0;
    switch(*ref) {
        case '-':
            (*root)->operator = '-';
            (*root)->leftChild = 0;
            parseForm((ref+1), &((*root)->rightChild));
            return;
            break;
        case 'A':
            if( *(ref+1) == '[') {
                if (*(ref+2) == 'x' || *(ref+2) == 'y' || *(ref+2) == 'z') {
                   if (*(ref+3) == 'x' || *(ref+3) == 'y' || *(ref+3) == 'z') {
                        if (*(ref+4) == ']') {
                            (*root)->operator = 'A';
                            (*root)->leftChild = 0;
                            (*root)->rightChild = 0;
                            return;
                        } else fprintf(stderr, "ERROR: Unknown character found.\n");
                   } else fprintf(stderr, "ERROR: Unknown character found.\n");
                } else fprintf(stderr, "ERROR: Unknown character found.\n");
            } else fprintf(stderr, "ERROR: Unknown character found.\n");
            break;
        case 'E':
            if( *(ref+1) == '[') {
                if (*(ref+2) == 'x' || *(ref+2) == 'y' || *(ref+2) == 'z') {
                   if (*(ref+3) == 'x' || *(ref+3) == 'y' || *(ref+3) == 'z') {
                        if (*(ref+4) == ']') {
                            (*root)->operator = 'E';
                            (*root)->leftChild = 0;
                            (*root)->rightChild = 0;
                            return;
                        } else fprintf(stderr, "ERROR: Unknown character found.\n");
                   } else fprintf(stderr, "ERROR: Unknown character found.\n");
                } else fprintf(stderr, "ERROR: Unknown character found.\n");
            } else fprintf(stderr, "ERROR: Unknown character found.\n");
            break;
        case '(':

            while(*ref) {
                if(*ref == '(') scope++;

                if((op != '\0') && (*ref == 'A' || *ref == 'E' || *ref == '(')) second = ref;

                if((scope == 1) && (*ref == 'v')) op = 'v';
                if((scope == 1) && (*ref == '^')) op = '^';
                if((scope == 1) && (*ref == '>')) op = '>';
                if(*ref == ')' && scope == 1) break;
                if(*ref == ')') scope--;
                ref++;
            }
            if(scope != 1) fprintf(stderr, "ERROR: Invalid Formula.\n"), exit(1);
            (*root)->operator = op;
            parseForm((start+1), &((*root)->leftChild));
            if(second != 0) parseForm(second, &((*root)->rightChild));
            else (*root)->rightChild = 0;
            return;
            break;
        default:
            free((*root));
            (*root) = 0;
            return;
    }
}


void printParseTree(Node *root) {
    static int indentNo = 0;
    indentNo++;
    int curIndent = indentNo;
    int i;
    if (root == 0) return;

    for (i = 0; i< curIndent; i++) fprintf(stdout, "  ");
    fprintf(stdout, "operator: %c\n", root->operator);

    for (i = 0; i< curIndent; i++) fprintf(stdout, "  ");
        fprintf(stdout, "leftChild:\n");


            printParseTree(root->leftChild);

    for (i = 0; i< curIndent; i++) fprintf(stdout, "  ");
        fprintf(stdout, "rightChild:\n");


            printParseTree(root->rightChild);

    for (i = 0; i< curIndent; i++) fprintf(stdout, "  ");
    fprintf(stdout, "End of Tree\n");
    return;
}
