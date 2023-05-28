#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float real;
    float imag;
} complex;

void produto_matricial(complex** a, complex** b, complex** result, int l, int c, int m) {
    int i, j, k;
    complex temp;

    for (i = 0; i < l; i++) {
        for (j = 0; j < m; j++) {
            temp.real = 0;
            temp.imag = 0;
            for (k = 0; k < c; k++) {
                temp.real += a[i][k].real * b[k][j].real - a[i][k].imag * b[k][j].imag;
                temp.imag += a[i][k].real * b[k][j].imag + a[i][k].imag * b[k][j].real;
            }
            result[i][j] = temp;
        }
    }
}

int main() {
    int l, c, m, i, j;

    printf("Digite o numero de linhas da matriz A: ");
    scanf("%d", &l);
    printf("Digite o numero de colunas da matriz A: ");
    scanf("%d", &c);
    printf("Digite o numero de linhas da matriz B: ");
    scanf("%d", &m);
    printf("Digite o numero de colunas da matriz B: ");
    scanf("%d", &m);

    complex** a = (complex**) malloc(l * sizeof(complex*));
    complex** b = (complex**) malloc(c * sizeof(complex*));
    complex** result = (complex**) malloc(l * sizeof(complex*));

    for (i = 0; i < l; i++) {
        a[i] = (complex*) malloc(c * sizeof(complex));
    }

    for (i = 0; i < c; i++) {
        b[i] = (complex*) malloc(m * sizeof(complex));
    }

    for (i = 0; i < l; i++) {
        result[i] = (complex*) malloc(m * sizeof(complex));
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
    for (i = 0; i < c; i++) {
        for (j = 0; j < m; j++) {
            printf("b[%d][%d].real = ", i, j);
            scanf("%f", &b[i][j].real);
            printf("b[%d][%d].imag = ", i, j);
            scanf("%f", &b[i][j].imag);
        }
    }

    printf("Matriz A:\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("(%+.2f  %+.2fi)", a[i][j].real, a[i][j].imag);
		}
		printf("\n");
		}
		printf("\nMatriz B:\n");
		for (i = 0; i < c; i++) {
			for (j = 0; j < m; j++) {
				printf("(%+.2f  %+.2fi)", b[i][j].real, b[i][j].imag);
			}
			printf("\n");
		}

		produto_matricial(a, b, result, l, c, m);

		printf("\nMatriz Resultado:\n");
		for (i = 0; i < l; i++) {
			for (j = 0; j < m; j++) {
				printf("(%+.2f  %+.2fi)", result[i][j].real, result[i][j].imag);
			}
			printf("\n");
		}

		for (i = 0; i < l; i++) {
			free(a[i]);
			free(result[i]);
		}
		for (i = 0; i < c; i++) {
			free(b[i]);
		}

		free(a);
		free(b);
		free(result);

		return 0;
	}
