#ifndef MATRIZES_H
#define MATRIZES_H

typedef struct {
    float real;
    float imag;
} complex;

void transposta(complex** a, complex** result, int l, int c);
void conjugada(complex** a, complex** result, int l, int c);
void hermitiana(complex** a, complex** result, int l, int c);
void soma(complex** a, complex** b, complex** result, int l, int c);
void subtracao(complex** a, complex** b, complex** result, int l, int c);
void produto_escalar(complex* a, complex* b, complex* result, int tam);
void produto_matricial(complex** a, complex** b, complex** result, int l, int c, int m);
void dois_exemplos(void);
void teste_todos(void);

#endif // MATRIZES_H
