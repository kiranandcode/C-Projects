#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <stdlib.h>
#define CMPLX(a,b) (((double complex) a) + ((double complex)b * I))

void print_complex(FILE * fp, double complex v) {
    fprintf(fp, "%.1f% + .1fi", creal(v), cimag(v));
}

void print_complex_array(FILE *fp, double complex *arr, int length) {
    fprintf(fp, "[\n");
    int i;
    for(i = 0; i<length; i++) {
        print_complex(fp, arr[i]);
        fprintf(fp, ",\n ");
    }
    fprintf(fp, "\b\b]\n");
}

double complex *cmplx(double *arr, int length) {
    double complex *result = malloc(sizeof(double complex) * length);
    int i;
    for(i = 0; i<length; i++) {
        result[i] = CMPLX(arr[i], 0.0);
    }
    return result;
}

double complex *recursive_fft(double complex *a, int length) {
    if(length == 1) return a;

    double complex wn = cexp(CMPLX(2*M_PI,0) * (I) /length);
    double complex w = 1;
    int even_length = (length % 2 == 0) ? length/2 : (length+1)/2;
    int odd_length = length - even_length;

    double complex *a0 = malloc(sizeof(double complex) * even_length);
    double complex *a1 = malloc(sizeof(double complex) * odd_length);
    int a0_count = 0;
    int a1_count = 0;
    int i;
    for(i = 0; i<length; i++) {
        if(i%2 == 0) {
            a0[a0_count++] = a[i];
        } else {
            a1[a1_count++] = a[i];
        }
    }

    double complex *y0 = recursive_fft(a0, even_length);
    double complex *y1 = recursive_fft(a1, odd_length);


    double complex *y = malloc(sizeof(double complex) * (length));

    for(i = 0; i<=length/2-1; i++) {
        y[i] = y0[i] + w * y1[i];
        y[i+(length/2)] = y0[i] - w * y1[i];
        w = w * wn;
    }



    return y;
}

int main() {
    // function  f(x) = 1 + 2*x + 3*x^2 + 4*x^3
    double *function = (double []){1.0,2.0,3.0,4.0};
    double complex *complexified = cmplx(function, 4);
    print_complex_array(stdout, complexified, 4);

    fprintf(stdout, "\n");

    double complex *dft = recursive_fft(complexified, 4);
    print_complex_array(stdout, dft, 4);
    fprintf(stdout, "\n");
    
    return 0;
}