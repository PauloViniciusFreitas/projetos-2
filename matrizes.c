/// @file matrizes.c
/// @brief Todas as implementações relacionadas a matrizes.

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float real;
    float imag;
} complex;
/// Essa função fará a transposição de uma matriz.

/// @param a Matriz de entrada.
/// @param result Matriz resultante (deve ser alocada antes da chamada).
/// @param l Número de linhas da matriz.
/// @param c Número de colunas da matriz.

void transposta(complex** a, complex** result, int l, int c) {
    int i, j;

    for (i = 0; i < c; i++) {
        for (j = 0; j < l; j++) {
            result[i][j].real = a[j][i].real;
            result[i][j].imag = a[j][i].imag;
        }
    }
}
/// Essa função calcula o conjugado de uma matriz.

/// @param a Matriz de entrada.
/// @param result Matriz resultante (deve ser alocada antes da chamada).
/// @param l Número de linhas da matriz.
/// @param c Número de colunas da matriz.

void conjugada(complex** a, complex** result, int l, int c) {
    int i, j;

    for (i = 0; i < c; i++) {
        for (j = 0; j < l; j++) {
            result[i][j].real = a[i][j].real;
            result[i][j].imag = -a[i][j].imag;
        }
    }
}
/// Essa função calcula a hermitiana (igual a sua transposta conjugada).

/// @param a Matriz de entrada.
/// @param result Matriz resultante (deve ser alocada antes da chamada).
/// @param l Número de linhas da matriz.
/// @param c Número de colunas da matriz.

void hermitiana(complex** a, complex** result, int l, int c) {
    int i, j;

    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            result[i][j].real = a[j][i].real;
            result[i][j].imag = -a[j][i].imag;
        }
    }
}
/// Essa função calcula a soma de duas matrizes complexas.

/// @param a Primeira matriz de entrada.
/// @param b Segunda matriz de entrada.
/// @param result Matriz resultante (deve ser alocada antes da chamada).
/// @param l Número de linhas das matrizes.
/// @param c Número de colunas das matrizes.

void soma(complex** a, complex** b, complex** result, int l, int c) {
    int i, j;

    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            result[i][j].real = a[i][j].real + b[i][j].real;
            result[i][j].imag = a[i][j].imag + b[i][j].imag;
        }
    }
}
/// Essa função calcula a subtração de duas matrizes complexas.

/// @param a Primeira matriz de entrada.
/// @param b Segunda matriz de entrada.
/// @param result Matriz resultante (deve ser alocada antes da chamada).
/// @param l Número de linhas das matrizes.
/// @param c Número de colunas das matrizes.

void subtracao(complex** a, complex** b, complex** result, int l, int c) {
    int i, j;

    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            result[i][j].real = a[i][j].real - b[i][j].real;
            result[i][j].imag = a[i][j].imag - b[i][j].imag;
        }
    }
}
/// Essa função calcula o produto escalar de dois vetores complexos.

/// @param a Primeiro vetor complexo de entrada.
/// @param b Segundo vetor complexo de entrada.
/// @param result Resultado do produto escalar (deve ser alocado antes da chamada).
/// @param tam Tamanho dos vetores.

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
/// Essa função calcula o produto matricial de duas matrizes.

/// @param a Primeira matriz de entrada.
/// @param b Segunda matriz de entrada.
/// @param result Matriz resultante (deve ser alocada antes da chamada).
/// @param l Número de linhas da primeira matriz.
/// @param c Número de colunas da primeira matriz e número de linhas da segunda matriz.
/// @param m Número de colunas da segunda matriz.

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
/// Essa função demonstra dois exemplos para cada operação com matrizes de 3 linhas por 3 colunas e um vetor de 3 posições (pro caso do produto escalar).

void dois_exemplos(void){
    int l, c, m, i, j;
    l=3;
    c=3;
    m=3;

    complex** a = (complex**) malloc(l * sizeof(complex*));
    complex** b = (complex**) malloc(l * sizeof(complex*));
    complex** b2 = (complex**) malloc(c * sizeof(complex*));
    complex* vetor_a = (complex*) malloc(l * sizeof(complex));
    complex* vetor_b = (complex*) malloc(l * sizeof(complex));
    complex** transposta_a = (complex**) malloc(c * sizeof(complex*));
    complex** conjugada_a = (complex**) malloc(l * sizeof(complex*));
    complex** hermitiana_a = (complex**) malloc(c * sizeof(complex*));
    complex** soma_a_b = (complex**) malloc(l * sizeof(complex*));
    complex** subtracao_a_b = (complex**) malloc(l * sizeof(complex*));
    complex produto_escalar_a_b = {0.0, 0.0};
    complex** produto_matricial_a_b = (complex**) malloc(l * sizeof(complex*));

    for (i = 0; i < l; i++) {
        a[i] = (complex*) malloc(c * sizeof(complex));
        b[i] = (complex*) malloc(c * sizeof(complex));
        soma_a_b[i] = (complex*) malloc(c * sizeof(complex));
        subtracao_a_b[i] = (complex*) malloc(c * sizeof(complex));
        produto_matricial_a_b[i] = (complex*) malloc(c * sizeof(complex));
    }

    for (i = 0; i < c; i++) {
        transposta_a[i] = (complex*) malloc(l * sizeof(complex));
        conjugada_a[i] = (complex*) malloc(l * sizeof(complex));
        b2[i] = (complex*) malloc(l * sizeof(complex));
        hermitiana_a[i] = (complex*) malloc(l * sizeof(complex));
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

    printf("Digite os valores para o vetor A:\n");
    for (i = 0; i < l; i++) {
        printf("Digite o valor de a[%d]:\n", i);
        scanf("%f%f", &vetor_a[i].real, &vetor_a[i].imag);
    }

    printf("Digite os valores para o vetor B:\n");
    for (i = 0; i < l; i++) {
        printf("Digite o valor de b[%d]:\n", i);
        scanf("%f%f", &vetor_b[i].real, &vetor_b[i].imag);
    }

    transposta(a, transposta_a, l, c);
    conjugada(a, conjugada_a, l, c);
    hermitiana(a, hermitiana_a, l, c);
    soma(a, b, soma_a_b, l, c);
    subtracao(a, b, subtracao_a_b, l, c);
    produto_escalar(vetor_a, vetor_b, &produto_escalar_a_b, l);
    produto_matricial(a, b, produto_matricial_a_b, l, c, m);

    printf("\n");

    printf("Matriz A:\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%+.2f  %+.2fi\t", a[i][j].real, a[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");

    printf("Matriz B:\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%+.2f  %+.2fi\t", b[i][j].real, b[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");

    printf("======Teste da operacao Transposta======\n");
    printf("Operando: A=\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%+.2f  %+.2fi\t", a[i][j].real, a[i][j].imag);
        }
        printf("\n");
    }
    printf("Resultado: R=\n");
    for (i = 0; i < c; i++) {
        for (j = 0; j < l; j++) {
            printf("%.2f  %.2fi\t", transposta_a[i][j].real, transposta_a[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");

    printf("Operando: A2=\n");
    for (i = 2; i >= 0; i--) {
        for (j = 2; j >= 0; j--) {
            printf("%+.2f  %+.2fi\t", a[i][j].real, a[i][j].imag);
        }
        printf("\n");
    }
    printf("Resultado: R2=\n");
    for (i = 2; i >= 0; i--) {
        for (j = 2; j >= 0; j--) {
            printf("%.2f  %.2fi\t", transposta_a[i][j].real, transposta_a[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");


    printf("======Teste da operacao Conjugada======\n");
    printf("Operando: A=\n");
        for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%+.2f  %+.2fi\t", a[i][j].real, a[i][j].imag);
        }
        printf("\n");
    }
    printf("Resultado: R=\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%.2f  %.2fi\t", conjugada_a[i][j].real, conjugada_a[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");

    printf("Operando: A2=\n");
    for (i = 2; i >= 0; i--) {
        for (j = 2; j >= 0; j--) {
            printf("%+.2f  %+.2fi\t", a[i][j].real, a[i][j].imag);
        }
        printf("\n");
    }
    printf("Resultado: R2=\n");
    for (i = 2; i >= 0; i--) {
        for (j = 2; j >= 0; j--) {
            printf("%.2f  %.2fi\t", conjugada_a[i][j].real, conjugada_a[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");

    printf("======Teste da operacao Hermitiana======\n");
    printf("Operando: A=\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%+.2f  %+.2fi\t", a[i][j].real, a[i][j].imag);
        }
        printf("\n");
    }
    printf("Resultado: R=\n");
    for (i = 0; i < c; i++) {
        for (j = 0; j < l; j++) {
            printf("%.2f  %.2fi\t", hermitiana_a[i][j].real, hermitiana_a[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");

    printf("Operando: A2=\n");
    for (i = 2; i >= 0; i--) {
        for (j = 2; j >= 0; j--) {
            printf("%+.2f  %+.2fi\t", a[i][j].real, a[i][j].imag);
        }
        printf("\n");
    }
    printf("Resultado: R2=\n");
    for (i = 2; i >= 0; i--) {
        for (j = 2; j >= 0; j--) {
            printf("%.2f  %.2fi\t", hermitiana_a[i][j].real, hermitiana_a[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");

    printf("======Teste da operacao Soma======\n");
    printf("Operando: A=\n");
        for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%+.2f  %+.2fi\t", a[i][j].real, a[i][j].imag);
        }
        printf("\n");
    }
    printf("Operando B:\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%+.2f  %+.2fi\t", b[i][j].real, b[i][j].imag);
        }
        printf("\n");
    }
    printf("Resultado: R=\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%.2f  %.2fi\t", soma_a_b[i][j].real, soma_a_b[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");

    printf("Operando: A2=\n");
    for (i = 2; i >= 0; i--) {
        for (j = 2; j >= 0; j--) {
            printf("%+.2f  %+.2fi\t", a[i][j].real, a[i][j].imag);
        }
        printf("\n");
    }
    printf("Operando: B2=\n");
    for (i = 2; i >= 0; i--) {
        for (j = 2; j >= 0; j--) {
            printf("%+.2f  %+.2fi\t", b[i][j].real, b[i][j].imag);
        }
        printf("\n");
    }
    printf("Resultado: R2=\n");
    for (i = 2; i >= 0; i--) {
        for (j = 2; j >= 0; j--) {
            printf("%.2f  %.2fi\t", soma_a_b[i][j].real, soma_a_b[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");

    printf("======Teste da operacao Subtracao======\n");
    printf("Operando: A=\n");
        for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%+.2f  %+.2fi\t", a[i][j].real, a[i][j].imag);
        }
        printf("\n");
    }
    printf("Operando B:\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%+.2f  %+.2fi\t", b[i][j].real, b[i][j].imag);
        }
        printf("\n");
    }
    printf("Resultado: R=\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%.2f  %.2fi\t", subtracao_a_b[i][j].real, subtracao_a_b[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");

    printf("Operando: A2=\n");
    for (i = 2; i >= 0; i--) {
        for (j = 2; j >= 0; j--) {
            printf("%+.2f  %+.2fi\t", a[i][j].real, a[i][j].imag);
        }
        printf("\n");
    }
    printf("Operando: B2=\n");
    for (i = 2; i >= 0; i--) {
        for (j = 2; j >= 0; j--) {
            printf("%+.2f  %+.2fi\t", b[i][j].real, b[i][j].imag);
        }
        printf("\n");
    }
    printf("Resultado: R2=\n");
    for (i = 2; i >= 0; i--) {
        for (j = 2; j >= 0; j--) {
            printf("%.2f  %.2fi\t", subtracao_a_b[i][j].real, subtracao_a_b[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");

    printf("======Teste da operacao Produto Escalar======\n");
    printf("Operando: A=\n");
    for (i = 0; i < l; i++) {
        printf("(%+.2f  %+.2fi)\t", vetor_a[i].real, vetor_a[i].imag);
    }

    printf("\n");

    printf("Operando: B=\n");
    for (i = 0; i < l; i++) {
        printf("(%+.2f  %+.2fi)\t", vetor_b[i].real, vetor_b[i].imag);
    }

    printf("\n");

    printf("Resultado: R=\n");
    printf("%.2f  %.2fi\n", produto_escalar_a_b.real, produto_escalar_a_b.imag);

    printf("\n");

    printf("Operando: A2=\n");
    for (i = 2; i >= 0; i--) {
        printf("(%+.2f  %+.2fi)\t", vetor_a[i].real, vetor_a[i].imag);
    }

    printf("\n");

    printf("Operando: B2=\n");
    for (i = 2; i >= 0; i--) {
        printf("(%+.2f  %+.2fi)\t", vetor_b[i].real, vetor_b[i].imag);
    }

    printf("\n");

    printf("Resultado: R2=\n");
    for (i = 0; i >= 0; i--) {
        printf("%.2f  %.2fi\n", produto_escalar_a_b.real, produto_escalar_a_b.imag);
    }

    printf("\n");

    printf("======Teste da operacao Produto Matricial======\n");
    printf("Operando: A=\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%+.2f  %+.2fi\t", a[i][j].real, b[i][j].imag);
        }
        printf("\n");
    }
    printf("Operando: B=\n");
    for (i = 0; i < c; i++) {
        for (j = 0; j < m; j++) {
            printf("(%+.2f  %+.2fi)", b[i][j].real, b[i][j].imag);
        }
        printf("\n");
    }
    printf("Resultado: R=\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < m; j++) {
            printf("(%.2f  %.2fi)", produto_matricial_a_b[i][j].real, produto_matricial_a_b[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");

    printf("Operando: A2=\n");
    for (i = 2; i >= 0; i--) {
        for (j = 2; j >= 0; j--) {
            printf("%+.2f  %+.2fi\t", a[i][j].real, b[i][j].imag);
        }
        printf("\n");
    }
    printf("Operando: B2=\n");
    for (i = 2; i >= 0; i--) {
        for (j = 2; j >= 0; j--) {
            printf("(%+.2f  %+.2fi)", b[i][j].real, b[i][j].imag);
        }
        printf("\n");
    }
    printf("Resultado: R2=\n");
    for (i = 2; i >= 0; i--) {
        for (j = 2; j >= 0; j--) {
            printf("(%.2f  %.2fi)", produto_matricial_a_b[i][j].real, produto_matricial_a_b[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");

    for (i = 0; i < l; i++) {
        free(a[i]);
        free(b[i]);
        free(soma_a_b[i]);
        free(subtracao_a_b[i]);
        free(produto_matricial_a_b[i]);
    }
    for (i = 0; i < c; i++) {
        free(b2[i]);
        free(transposta_a[i]);
        free(hermitiana_a[i]);
    }
    free(a);
    free(b);
    free(b2);
    free(soma_a_b);
    free(subtracao_a_b);
    free(vetor_a);
    free(vetor_b);
    free(conjugada_a);
    free(transposta_a);
    free(hermitiana_a);
    free(produto_matricial_a_b);
}
///Essa função executa todas as operações citadas ateriormente, mas com apenas um resultado.
void teste_todos(void) {
    int l, c, m, i, j;
    l=3;
    c=3;
    m=3;

    complex** a = (complex**) malloc(l * sizeof(complex*));
    complex** b = (complex**) malloc(l * sizeof(complex*));
    complex** b2 = (complex**) malloc(c * sizeof(complex*));
    complex* vetor_a = (complex*) malloc(l * sizeof(complex));
    complex* vetor_b = (complex*) malloc(l * sizeof(complex));
    complex** transposta_a = (complex**) malloc(c * sizeof(complex*));
    complex** conjugada_a = (complex**) malloc(l * sizeof(complex*));
    complex** hermitiana_a = (complex**) malloc(c * sizeof(complex*));
    complex** soma_a_b = (complex**) malloc(l * sizeof(complex*));
    complex** subtracao_a_b = (complex**) malloc(l * sizeof(complex*));
    complex produto_escalar_a_b = {0.0, 0.0};
    complex** produto_matricial_a_b = (complex**) malloc(l * sizeof(complex*));

    for (i = 0; i < l; i++) {
        a[i] = (complex*) malloc(c * sizeof(complex));
        b[i] = (complex*) malloc(c * sizeof(complex));
        soma_a_b[i] = (complex*) malloc(c * sizeof(complex));
        subtracao_a_b[i] = (complex*) malloc(c * sizeof(complex));
        produto_matricial_a_b[i] = (complex*) malloc(c * sizeof(complex));
    }

    for (i = 0; i < c; i++) {
        transposta_a[i] = (complex*) malloc(l * sizeof(complex));
        conjugada_a[i] = (complex*) malloc(l * sizeof(complex));
        b2[i] = (complex*) malloc(l * sizeof(complex));
        hermitiana_a[i] = (complex*) malloc(l * sizeof(complex));
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

    printf("Digite os valores para o vetor A:\n");
    for (i = 0; i < l; i++) {
        printf("Digite o valor de a[%d]:\n", i);
        scanf("%f%f", &vetor_a[i].real, &vetor_a[i].imag);
    }

    printf("Digite os valores para o vetor B:\n");
    for (i = 0; i < l; i++) {
        printf("Digite o valor de b[%d]:\n", i);
        scanf("%f%f", &vetor_b[i].real, &vetor_b[i].imag);
    }

    transposta(a, transposta_a, l, c);
    conjugada(a, conjugada_a, l, c);
    hermitiana(a, hermitiana_a, l, c);
    soma(a, b, soma_a_b, l, c);
    subtracao(a, b, subtracao_a_b, l, c);
    produto_escalar(vetor_a, vetor_b, &produto_escalar_a_b, l);
    produto_matricial(a, b, produto_matricial_a_b, l, c, m);
    

    printf("\n");

    printf("Matriz A:\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%+.2f  %+.2fi\t", a[i][j].real, a[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");

    printf("Matriz B:\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%+.2f  %+.2fi\t", b[i][j].real, b[i][j].imag);
        }
        printf("\n");
    }
    
    printf("\n");

    printf("=====Equipe=====\n");
    printf("Filipe Correa da Silva\n");
    printf("Paulo Vinicius de Freitas Rodrigues\n");
    printf("Kelton de Jesus Freitas Cantao\n");

    printf("\n");

    printf("======Teste da operacao Transposta======\n");
    printf("Operando: A=\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%+.2f  %+.2fi\t", a[i][j].real, a[i][j].imag);
        }
        printf("\n");
    }
    printf("Resultado: R=\n");
    for (i = 0; i < c; i++) {
        for (j = 0; j < l; j++) {
            printf("%.2f  %.2fi\t", transposta_a[i][j].real, transposta_a[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");

    printf("======Teste da operacao Conjugada======\n");
    printf("Operando: A=\n");
        for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%+.2f  %+.2fi\t", a[i][j].real, a[i][j].imag);
        }
        printf("\n");
    }
    printf("Resultado: R=\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%.2f  %.2fi\t", conjugada_a[i][j].real, conjugada_a[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");

    printf("======Teste da operacao Hermitiana======\n");
    printf("Operando: A=\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%+.2f  %+.2fi\t", a[i][j].real, a[i][j].imag);
        }
        printf("\n");
    }
    printf("Resultado: R=\n");
    for (i = 0; i < c; i++) {
        for (j = 0; j < l; j++) {
            printf("%.2f  %.2fi\t", hermitiana_a[i][j].real, hermitiana_a[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");

    printf("======Teste da operacao Soma======\n");
    printf("Operando: A=\n");
        for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%+.2f  %+.2fi\t", a[i][j].real, a[i][j].imag);
        }
        printf("\n");
    }
    printf("Operando B:\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%+.2f  %+.2fi\t", b[i][j].real, b[i][j].imag);
        }
        printf("\n");
    }
    printf("Resultado: R=\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%.2f  %.2fi\t", soma_a_b[i][j].real, soma_a_b[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");
    
    printf("======Teste da operacao Subtracao======\n");
    printf("Operando: A=\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%+.2f  %+.2fi\t", a[i][j].real, a[i][j].imag);
        }
        printf("\n");
    }
    printf("Operando B:\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%+.2f  %+.2fi\t", b[i][j].real, b[i][j].imag);
        }
        printf("\n");
    }
    printf("Resultado: R=\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%.2f  %.2fi\t", subtracao_a_b[i][j].real, subtracao_a_b[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");
    
    printf("======Teste da operacao Produto Escalar======\n");
    printf("Operando: A=\n");
    for (i = 0; i < l; i++) {
        printf("(%+.2f  %+.2fi)\t", vetor_a[i].real, vetor_a[i].imag);
    }

    printf("\n");

    printf("Operando: B=\n");
    for (i = 0; i < l; i++) {
        printf("(%+.2f  %+.2fi)\t", vetor_b[i].real, vetor_b[i].imag);
    }

    printf("\n");

    printf("Resultado: R=\n");
    printf("%.2f  %.2fi\n", produto_escalar_a_b.real, produto_escalar_a_b.imag);

    printf("\n");

    printf("======Teste da operacao Produto Matricial======\n");
    printf("Operando: A=\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            printf("%+.2f  %+.2fi\t", a[i][j].real, b[i][j].imag);
        }
        printf("\n");
    }
    printf("Operando: B=\n");
    for (i = 0; i < c; i++) {
        for (j = 0; j < m; j++) {
            printf("(%+.2f  %+.2fi)", b[i][j].real, b[i][j].imag);
        }
        printf("\n");
    }
    printf("Resultado: R=\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < m; j++) {
            printf("(%.2f  %.2fi)", produto_matricial_a_b[i][j].real, produto_matricial_a_b[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");

    for (i = 0; i < l; i++) {
        free(a[i]);
        free(b[i]);
        free(soma_a_b[i]);
        free(subtracao_a_b[i]);
        free(produto_matricial_a_b[i]);
    }
    for (i = 0; i < c; i++) {
        free(b2[i]);
        free(transposta_a[i]);
        free(hermitiana_a[i]);
    }
    free(a);
    free(b);
    free(b2);
    free(soma_a_b);
    free(subtracao_a_b);
    free(vetor_a);
    free(vetor_b);
    free(conjugada_a);
    free(transposta_a);
    free(hermitiana_a);
    free(produto_matricial_a_b);
}
//int main(void) {
//    dois_exemplos();
//    return 0;
//}
