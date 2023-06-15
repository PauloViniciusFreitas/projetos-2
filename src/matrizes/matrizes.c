/// @file matrizes.c
/// @brief Todas as implementações relacionadas a matrizes.

#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_linalg.h>

///número de linhas gerais usadas pelas matrizes e vetores no cálculo da técnica de decomposição svd.
#define M 6

///número de colunas gerais usadas pelas matrizes e vetores no cálculo da técnica de decomposição svd
#define n 6

///elementos das matrizes e vetores da função svd.

double a_data[M][n] = {
    {1, 2, 3, 4, 5, 6},
    {7, 8, 9, 10, 11, 12 },
    {13, 14, 15, 16, 17, 18},
    {19, 20, 21, 22, 23, 24},
    {25, 26, 27, 28, 29, 30},
    {31, 32, 33, 34, 35, 36}
};

typedef struct {
    float real;
    float imag;
} complex;
///A transposição de uma matriz envolve a troca de suas linhas pelas colunas correspondentes. Isso significa que o elemento (i, j) da matriz original será colocado como elemento (j, i) na matriz transposta.

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
///A conjugada de uma matriz é obtida trocando o sinal da parte imaginária de cada elemento complexo da matriz.

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
///Uma matriz hermitiana é uma matriz complexa que é igual à sua matriz conjugada transposta. Isso significa que os elementos (i, j) da matriz original são iguais aos elementos (j, i) da matriz conjugada transposta.

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
///Para somar duas matrizes complexas, você deve somar separadamente a parte real e a parte imaginária de cada elemento correspondente das matrizes. O resultado será uma nova matriz complexa com a soma das partes reais e a soma das partes imaginárias.

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
///Para subtrair duas matrizes complexas, você deve subtrair separadamente a parte real e a parte imaginária de cada elemento correspondente das matrizes. O resultado será uma nova matriz complexa com a diferença das partes reais e a diferença das partes imaginárias.

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
///a operação de produto escalar entre dois vetores complexos consiste em multiplicar a parte real de A pelo correspondente da parte real de B, multiplicar a parte imaginária de A pelo correspondente da parte imaginária de B, subtrair a multiplicação das partes imaginárias de A e B da multiplicação das partes reais de A e B, e somar o resultado à variável temp.real. Além disso, deve-se multiplicar a parte real de A pelo correspondente da parte imaginária de B, multiplicar a parte imaginária de A pelo correspondente da parte real de B, e somar o resultado à variável temp.imag.

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
///Para multiplicar duas matrizes complexas, você deve aplicar a regra geral de multiplicação de matrizes, considerando que os números complexos têm uma parte real e uma parte imaginária. Isso envolve multiplicar os elementos correspondentes, somar os produtos cruzados das partes reais e imaginárias e combiná-los para obter a parte real e a parte imaginária do resultado.

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

///A função gsl_linalg_SV_decomp realiza a decomposição em valores singulares (Singular Value Decomposition - SVD) da matriz A. Essa função é chamada com os argumentos A, V, S e work para realizar a decomposição, calcula em três partes principais: matriz U, matriz V e vetor de valores singulares S. Os resultados são armazenados nas matrizes e vetores passados como argumentos. Em seguida, as matrizes V, U e o vetor S são exibidos no console. A SVD permite decompor uma matriz complexa em componentes mais simples, fornecendo informações sobre sua estrutura e propriedades.

void calc_svd(void) {
    gsl_matrix *A = gsl_matrix_alloc(M, n);
    gsl_matrix *V = gsl_matrix_alloc(n, n);
    gsl_vector *S = gsl_vector_alloc(n);
    gsl_vector *work = gsl_vector_alloc(n);
}

/// Essa função demonstra dois exemplos para cada operação com matrizes de 3 linhas por 3 colunas e um vetor de 3 posições (pro caso do produto escalar) e também o cálculo da svd.

void dois_exemplos(void){
    gsl_matrix *A = gsl_matrix_alloc(M, n);
    gsl_matrix *V = gsl_matrix_alloc(n, n);
    gsl_vector *S = gsl_vector_alloc(n);
    gsl_vector *work = gsl_vector_alloc(n);

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

    printf("======Teste da funcao Calculo SVD======\n");

    printf("Matriz Complexa:\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < 2; j++) {
            printf("%.2f  %.2fi\t", a[i][j].real, a[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");

    printf("\nAviso: A matriz é complexa. Logo, será calculada a SVD somente da parte real da matriz:\n");

    printf("\n\n");

    printf("Matriz A(SVD 3x2)\n");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++) {
            printf("%.2f ", a[i][j].real);
            gsl_matrix_set(A, i, j, a[i][j].real);
        }
        printf("\n");
    }

    gsl_linalg_SV_decomp(A, V, S, work);

    printf("\n\nMatriz V\n");
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            printf("%.2f ", gsl_matrix_get(V, i, j));
        }
        printf("\n");
    }

    printf("\n\nMatriz U\n");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++) {
            printf("%.2f ", gsl_matrix_get(A, i, j));
        }
        printf("\n");
    }

    printf("\n\nVetor S\n");
    for (i = 0; i < 2; i++)
        printf("%.2f ", gsl_vector_get(S, i));
    printf("\n");

    printf("\n\n");

    printf("Matriz A(SVD 4x4)\n");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            printf("%.2f ", a_data[i][j]);
            gsl_matrix_set(A, i, j, a_data[i][j]);
        }
        printf("\n");
    }

    gsl_linalg_SV_decomp(A, V, S, work);

    printf("\n\nMatriz V\n");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            printf("%.2f ", gsl_matrix_get(V, i, j));
        }
        printf("\n");
    }

    printf("\n\nMatriz U\n");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            printf("%.2f ", gsl_matrix_get(A, i, j));
        }
        printf("\n");
    }

    printf("\n\nVetor S\n");
    for (i = 0; i < 4; i++)
        printf("%.2f ", gsl_vector_get(S, i));
    printf("\n");

    printf("\n\n");

    printf("Matriz A(SVD 6x5)\n");
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 5; j++) {
            printf("%.2f ", a_data[i][j]);
            gsl_matrix_set(A, i, j, a_data[i][j]);
        }
        printf("\n");
    }

    gsl_linalg_SV_decomp(A, V, S, work);

    printf("\n\nMatriz V\n");
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            printf("%.2f ", gsl_matrix_get(V, i, j));
        }
        printf("\n");
    }

    printf("\n\nMatriz U\n");
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 5; j++) {
            printf("%.2f ", gsl_matrix_get(A, i, j));
        }
        printf("\n");
    }

    printf("\n\nVetor S\n");
    for (i = 0; i < 5; i++)
        printf("%.2f ", gsl_vector_get(S, i));
    printf("\n");

    printf("\n\n");

    printf("Matriz A(SVD 5x6)\n");
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 6; j++) {
            printf("%.2f ", a_data[i][j]);
            gsl_matrix_set(A, i, j, a_data[i][j]);
        }
        printf("\n");
    }

    gsl_linalg_SV_decomp(A, V, S, work);

    printf("\n\nMatriz V\n");
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
            printf("%.2f ", gsl_matrix_get(V, i, j));
        }
        printf("\n");
    }

    printf("\n\nMatriz U\n");
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 6; j++) {
            printf("%.2f ", gsl_matrix_get(A, i, j));
        }
        printf("\n");
    }

    printf("\n\nVetor S\n");
    for (i = 0; i < 6; i++)
        printf("%.2f ", gsl_vector_get(S, i));
    printf("\n\n");

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
    gsl_matrix_free(A);
    gsl_matrix_free(V);
    gsl_vector_free(S);
    gsl_vector_free(work);
}

///Essa função executa todas as operações matriciais e vetoriais citadas ateriormente, mas com apenas um único resultado. também, faz a svd, com dimenções de 3x2, 4x4, 6x5 e 5x6, sendo que a primeira matriz(de dimenções 3x2) tem como entrada de elementos os números complexos, mas usa-se somente sua parte real.
void teste_todos(void) {
    gsl_matrix *A = gsl_matrix_alloc(M, n);
    gsl_matrix *V = gsl_matrix_alloc(n, n);
    gsl_vector *S = gsl_vector_alloc(n);
    gsl_vector *work = gsl_vector_alloc(n);

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

    a[0][0].real = 1;
    a[0][0].imag = 2;
    a[0][1].real = 3;
    a[0][1].imag = 4;
    a[0][2].real = 5;
    a[0][2].imag = 6;
    a[1][0].real = 7;
    a[1][0].imag = 8;
    a[1][1].real = 9;
    a[1][1].imag = 10;
    a[1][2].real = 11;
    a[1][2].imag = 12;
    a[2][0].real = 13;
    a[2][0].imag = 14;
    a[2][1].real = 15;
    a[2][1].imag = 16;
    a[2][2].real = 17;
    a[2][2].imag = 18;

    b[0][0].real = 1;
    b[0][0].imag = 2;
    b[0][1].real = 3;
    b[0][1].imag = 4;
    b[0][2].real = 5;
    b[0][2].imag = 6;
    b[1][0].real = 7;
    b[1][0].imag = 8;
    b[1][1].real = 9;
    b[1][1].imag = 10;
    b[1][2].real = 11;
    b[1][2].imag = 12;
    b[2][0].real = 13;
    b[2][0].imag = 14;
    b[2][1].real = 15;
    b[2][1].imag = 16;
    b[2][2].real = 17;
    b[2][2].imag = 18;
    
    vetor_a[0].real=1;
    vetor_a[0].imag=2;
    vetor_a[1].real=3;
    vetor_a[1].imag=4;
    vetor_a[2].real=5;
    vetor_a[2].imag=6;

    vetor_b[0].real=1;
    vetor_b[0].imag=2;
    vetor_b[1].real=3;
    vetor_b[1].imag=4;
    vetor_b[2].real=5;
    vetor_b[2].imag=6;

    transposta(a, transposta_a, l, c);
    conjugada(a, conjugada_a, l, c);
    hermitiana(a, hermitiana_a, l, c);
    soma(a, b, soma_a_b, l, c);
    subtracao(a, b, subtracao_a_b, l, c);
    produto_escalar(vetor_a, vetor_b, &produto_escalar_a_b, l);
    produto_matricial(a, b, produto_matricial_a_b, l, c, m);

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

    printf("======Teste da funcao Calculo SVD======\n");

    printf("Matriz Complexa:\n");
    for (i = 0; i < l; i++) {
        for (j = 0; j < 2; j++) {
            printf("%.2f  %.2fi\t", a[i][j].real, a[i][j].imag);
        }
        printf("\n");
    }

    printf("\n");

    printf("\nAviso: A matriz é complexa. Logo, será calculada a SVD somente da parte real da matriz:\n");

    printf("\n\n");

    printf("Matriz A(SVD 3x2)\n");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++) {
            printf("%.2f ", a[i][j].real);
            gsl_matrix_set(A, i, j, a[i][j].real);
        }
        printf("\n");
    }

    gsl_linalg_SV_decomp(A, V, S, work);

    printf("\n\nMatriz V\n");
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            printf("%.2f ", gsl_matrix_get(V, i, j));
        }
        printf("\n");
    }

    printf("\n\nMatriz U\n");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++) {
            printf("%.2f ", gsl_matrix_get(A, i, j));
        }
        printf("\n");
    }

    printf("\n\nVetor S\n");
    for (i = 0; i < 2; i++)
        printf("%.2f ", gsl_vector_get(S, i));
    printf("\n");

    printf("\n\n");

    printf("Matriz A(SVD 4x4)\n");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            printf("%.2f ", a_data[i][j]);
            gsl_matrix_set(A, i, j, a_data[i][j]);
        }
        printf("\n");
    }

    gsl_linalg_SV_decomp(A, V, S, work);

    printf("\n\nMatriz V\n");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            printf("%.2f ", gsl_matrix_get(V, i, j));
        }
        printf("\n");
    }

    printf("\n\nMatriz U\n");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            printf("%.2f ", gsl_matrix_get(A, i, j));
        }
        printf("\n");
    }

    printf("\n\nVetor S\n");
    for (i = 0; i < 4; i++)
        printf("%.2f ", gsl_vector_get(S, i));
    printf("\n");

    printf("\n\n");

    printf("Matriz A(SVD 6x5)\n");
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 5; j++) {
            printf("%.2f ", a_data[i][j]);
            gsl_matrix_set(A, i, j, a_data[i][j]);
        }
        printf("\n");
    }

    gsl_linalg_SV_decomp(A, V, S, work);

    printf("\n\nMatriz V\n");
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            printf("%.2f ", gsl_matrix_get(V, i, j));
        }
        printf("\n");
    }

    printf("\n\nMatriz U\n");
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 5; j++) {
            printf("%.2f ", gsl_matrix_get(A, i, j));
        }
        printf("\n");
    }

    printf("\n\nVetor S\n");
    for (i = 0; i < 5; i++)
        printf("%.2f ", gsl_vector_get(S, i));
    printf("\n");

    printf("\n\n");

    printf("Matriz A(SVD 5x6)\n");
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 6; j++) {
            printf("%.2f ", a_data[i][j]);
            gsl_matrix_set(A, i, j, a_data[i][j]);
        }
        printf("\n");
    }

    gsl_linalg_SV_decomp(A, V, S, work);

    printf("\n\nMatriz V\n");
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
            printf("%.2f ", gsl_matrix_get(V, i, j));
        }
        printf("\n");
    }

    printf("\n\nMatriz U\n");
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 6; j++) {
            printf("%.2f ", gsl_matrix_get(A, i, j));
        }
        printf("\n");
    }

    printf("\n\nVetor S\n");
    for (i = 0; i < 6; i++)
        printf("%.2f ", gsl_vector_get(S, i));
    printf("\n\n");


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
    gsl_matrix_free(A);
    gsl_matrix_free(V);
    gsl_vector_free(S);
    gsl_vector_free(work);
}