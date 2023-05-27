#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float real;
    float imag;
} complex;

void produto_escalar(complex* a, complex* b, complex* result, int tam) {
    int i;
    complex temp = {0.0, 0.0};

    for (i = 0; i < tam; i++) {
        temp.real += a[i].real * b[i].real - a[i].imag * b[i].imag;
        temp.imag += a[i].real * b[i].imag + a[i].imag * b[i].real;
    }
    result->real = temp.real;
    result->imag = temp.imag;
}

int main() {
    int tam, i;

    printf("Digite o tamanho dos vetores: ");
    scanf("%d", &tam);

    complex* a = (complex*) malloc(tam * sizeof(complex));
    complex* b = (complex*) malloc(tam * sizeof(complex));
    complex result = {0.0, 0.0};

    printf("Digite os valores do vetor A:\n");
    for (i = 0; i < tam; i++) {
        printf("a[%d].real = ", i);
        scanf("%f", &a[i].real);
        printf("a[%d].imag = ", i);
        scanf("%f", &a[i].imag);
    }

    printf("Digite os valores do vetor B:\n");
    for (i = 0; i < tam; i++) {
        printf("b[%d].real = ", i);
        scanf("%f", &b[i].real);
        printf("b[%d].imag = ", i);
        scanf("%f", &b[i].imag);
    }

    printf("Vetor A:\n");
    for (i = 0; i < tam; i++) {
        printf("(%+.2f  %+.2fi)\t", a[i].real, a[i].imag);
    }
    printf("\n");

    printf("Vetor B:\n");
    for (i = 0; i < tam; i++) {
        printf("(%+.2f  %+.2fi)\t", b[i].real, b[i].imag);
    }
    printf("\n");

    produto_escalar(a, b, &result, tam);

    printf("\nProduto escalar de A e B: (%.2f + %.2fi)\n", result.real, result.imag);

    free(a);
    free(b);

    return 0;
}
