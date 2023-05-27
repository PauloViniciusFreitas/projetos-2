#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float real;
    float imag;
} complex;

void conjugada(complex** a, complex** result, int l, int c) {
    int i, j;

    for (i = 0; i < c; i++) {
        for (j = 0; j < l; j++) {
            result[i][j].real = a[i][j].real;
            result[i][j].imag = -a[i][j].imag;
        }
    }
}

int main() {
    int l, c, i, j;

    printf("Digite o numero de linhas: ");
    scanf("%d", &l);
    printf("Digite o numero de colunas: ");
    scanf("%d", &c);

    complex** a = (complex**) malloc(l * sizeof(complex*));
    complex** result = (complex**) malloc(c * sizeof(complex*));
    for (i = 0; i < l; i++) {
        a[i] = (complex*) malloc(c * sizeof(complex));
    }
    for (i = 0; i < c; i++) {
        result[i] = (complex*) malloc(l * sizeof(complex));
    }

    printf("Digite os valores da matriz A:\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("a[%d][%d].real = ", i, j);
            scanf("%f", &a[i][j].real);
            printf("a[%d][%d].imag = ", i, j);
            scanf("%f", &a[i][j].imag);
        }
    }
    printf("Matriz Original:\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("(%+.2f  %+.2fi)\t", a[i][j].real, a[i][j].imag);
        }
        printf("\n");
    }
    conjugada(a, result, l, c);

    printf("Matriz Conjugada:\n");
    for (i = 0; i < c; i++) {
        for (j = 0; j < l; j++) {
            printf("(%+.2f  %+.2fi)\t", result[i][j].real, result[i][j].imag);
        }
        printf("\n");
    }

    for (i = 0; i < l; i++) {
        free(a[i]);
    }
    for (i = 0; i < c; i++) {
        free(result[i]);
    }
    free(a);
    free(result);

    return 0;
}
