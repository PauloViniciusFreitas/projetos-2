#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float real;
    float imag;
} complex;

void soma(complex** a, complex** b, complex** result, int l, int c) {
    int i, j;

    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            result[i][j].real = a[i][j].real + b[i][j].real;
            result[i][j].imag = a[i][j].imag + b[i][j].imag;
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
    complex** b = (complex**) malloc(l * sizeof(complex*));
    complex** result = (complex**) malloc(l * sizeof(complex*));

    for (i = 0; i < l; i++) {
        a[i] = (complex*) malloc(c * sizeof(complex));
        b[i] = (complex*) malloc(c * sizeof(complex));
        result[i] = (complex*) malloc(c * sizeof(complex));
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

    printf("Digite os valores da matriz B:\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("b[%d][%d].real = ", i, j);
            scanf("%f", &b[i][j].real);
            printf("b[%d][%d].imag = ", i, j);
            scanf("%f", &b[i][j].imag);
        }
    }

    printf("Matriz A:\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("(%+.2f  %+.2fi)\t", a[i][j].real, a[i][j].imag);
        }
        printf("\n");
    }

    printf("Matriz B:\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("(%+.2f  %+.2fi)\t", b[i][j].real, b[i][j].imag);
        }
        printf("\n");
    }
    soma(a, b, result, l, c);

    printf("Matriz Resultado:\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("(%+.2f  %+.2fi)\t", result[i][j].real, result[i][j].imag);
        }
        printf("\n");
    }

    for (i = 0; i < l; i++) {
        free(a[i]);
    }
    for (i = 0; i < l; i++) {
        free(b[i]);
    }
    for (i = 0; i < c; i++) {
        free(result[i]);
	}
	free(a);
	free(b);
	free(result);

	return 0;
}
