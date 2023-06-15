/// @file matrizes.h
/// @brief Definições e declarações de funções relacionadas a matrizes.

#ifndef MATRIZES_H
#define MATRIZES_H

/// @brief Estrutura que representa um número complexo.

typedef struct {
    float real; /**< Parte real do número complexo. */
    float imag; /**< Parte imaginária do número complexo. */
} complex;

/// @brief Calcula a matriz transposta.
 
/// @param a Matriz de entrada.
/// @param result Matriz resultante (deve ser alocada antes da chamada).
/// @param l Número de linhas da matriz.
/// @param c Número de colunas da matriz.

void transposta(complex** a, complex** result, int l, int c);

/// @brief Calcula a matriz conjugada.

/// @param a Matriz de entrada.
/// @param result Matriz resultante (deve ser alocada antes da chamada).
/// @param l Número de linhas da matriz.
/// @param c Número de colunas da matriz.

void conjugada(complex** a, complex** result, int l, int c);

/// @brief Calcula a matriz hermitiana.

/// @param a Matriz de entrada.
/// @param result Matriz resultante (deve ser alocada antes da chamada).
/// @param l Número de linhas da matriz.
/// @param c Número de colunas da matriz.

void hermitiana(complex** a, complex** result, int l, int c);

/// @brief Calcula a soma de duas matrizes.

/// @param a Primeira matriz de entrada.
/// @param b Segunda matriz de entrada.
/// @param result Matriz resultante (deve ser alocada antes da chamada).
/// @param l Número de linhas das matrizes.
/// @param c Número de colunas das matrizes.

void soma(complex** a, complex** b, complex** result, int l, int c);

/// @brief Calcula a subtração de duas matrizes.

/// @param a Primeira matriz de entrada.
/// @param b Segunda matriz de entrada.
/// @param result Matriz resultante (deve ser alocada antes da chamada).
/// @param l Número de linhas das matrizes.
/// @param c Número de colunas das matrizes.

void subtracao(complex** a, complex** b, complex** result, int l, int c);

/// @brief Calcula o produto escalar entre dois vetores complexos.

/// @param a Primeiro vetor complexo de entrada.
/// @param b Segundo vetor complexo de entrada.
/// @param result Resultado do produto escalar (deve ser alocado antes da chamada).
/// @param tam Tamanho dos vetores.

void produto_escalar(complex* a, complex* b, complex* result, int tam);

/// @brief Calcula o produto matricial entre duas matrizes.

/// @param a Primeira matriz de entrada.
/// @param b Segunda matriz de entrada.
/// @param result Matriz resultante (deve ser alocada antes da chamada).
/// @param l Número de linhas da primeira matriz.
/// @param c Número de colunas da primeira matriz e número de linhas da segunda matriz.
/// @param m Número de colunas da segunda matriz.

void produto_matricial(complex** a, complex** b, complex** result, int l, int c, int m);

void calc_svd(void);

/// @brief Executa dois exemplos de uso das funções.

void dois_exemplos();

/// @brief Executa testes em todas as funções.

void teste_todos();

#endif // MATRIZES_H
