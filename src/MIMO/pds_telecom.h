/// @file pds_telecom.h
/// @brief declaração de funções relacionadas ao sistema MIMO

#ifndef PDS_TELECOM_H
#define PDS_TELECOM_H

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <gsl/gsl_linalg.h>

/**
 * Lê os índices dos dados a serem transmitidos a partir de um arquivo.
 *
 * @param filename o nome do arquivo
 * @param size um ponteiro para armazenar o tamanho do array
 * @return um ponteiro para o array de inteiros lidos do arquivo
 */
int *tx_data_read(char *filename, int *size);

/**
 * Preenche os dados com zeros para atingir o tamanho desejado, considerando o número de streams e transmissores.
 *
 * @param data um ponteiro para o array de dados
 * @param size o tamanho do array de dados
 * @param num_streams o número de streams
 * @param Nt o número de transmissores
 * @return um ponteiro para o array de dados preenchidos
 */
double complex *tx_data_padding(double complex *data, int size, int num_streams, int Nt);

/**
 * Realiza o mapeamento dos índices para símbolos QAM.
 *
 * @param tx_indices um ponteiro para o array de índices
 * @param size o tamanho do array de índices
 * @return um ponteiro para o array de símbolos QAM
 */
double complex *QAMmapper(int *tx_indices, int size);

/**
 * Realiza o mapeamento em camada dos dados, dividindo-os em streams.
 *
 * @param data um ponteiro para o array de dados
 * @param size o tamanho do array de dados
 * @param num_streams o número de streams
 * @return um ponteiro para o array de dados mapeados em camadas
 */
double complex **tx_layer_mapper(double complex *data, int size, int num_streams);

/**
 * Gera uma matriz de canal aleatória.
 *
 * @param Nr o número de receptores
 * @param Nt o número de transmissores
 * @return um ponteiro para a matriz de canal gerada
 */
double **channel_gen(int Nr, int Nt);

/**
 * Transpõe a matriz de canal.
 *
 * @param H a matriz a ser transposta
 * @param Nr o número de receptores
 * @param Nt o número de transmissores
 * @return um ponteiro para a matriz transposta
 */
double **matrix_transpose(double **H, int Nr, int Nt);

/**
 * Realiza a transmissão dos dados pelo canal, adicionando ruído.
 *
 * @param data um ponteiro para o array de dados
 * @param size o tamanho do array de dados
 * @param num_streams o número de streams
 * @param H a matriz de canal
 * @param Nr o número de receptores
 * @param Nt o número de transmissores
 * @param ruido_min o valor mínimo do ruído
 * @param ruido_max o valor máximo do ruído
 * @return um ponteiro para o array de dados transmitidos
 */
double complex **channel_transmission(double complex **data, int size, int num_streams, double **H, int Nr, int Nt, double ruido_min, double ruido_max);

/**
 * Realiza a decomposição em valores singulares (SVD) da matriz transposta de canal.
 *
 * @param H a matriz a ser decomposta
 * @param Nr o número de receptores
 * @param Nt o número de transmissores
 * @param U um ponteiro para a matriz U resultante
 * @param S um ponteiro para o vetor S resultante
 * @param V um ponteiro para a matriz V resultante
 */
void svd(double **H, int Nr, int Nt, double **U, double *S, double **V);

/**
 * Pré-codifica os dados utilizando a matriz V resultante da decomposição SVD.
 *
 * @param data um ponteiro para o array de dados
 * @param size o tamanho do array de dados
 * @param num_streams o número de streams
 * @param V a matriz V da decomposição SVD
 * @return um ponteiro para o array de dados pré-codificados
 */
double complex **tx_precoder(double complex **data, int size, int num_streams, double **V);

/**
 * Combina os dados recebidos utilizando a matriz U resultante da decomposição SVD.
 *
 * @param data um ponteiro para o array de dados
 * @param size o tamanho do array de dados
 * @param num_streams o número de streams
 * @param U a matriz U da decomposição SVD
 * @return um ponteiro para o array de dados combinados
 */
double complex **rx_combiner(double complex **data, int size, int num_streams, double **U);

/**
 * Realiza o demapeamento em camada dos dados.
 *
 * @param data um ponteiro para o array de dados
 * @param size o tamanho do array de dados
 * @param num_streams o número de streams
 * @return um ponteiro para o array de dados demapeados em camadas
 */
double complex *rx_layer_demapper(double complex **data, int size, int num_streams);

/**
 * Realiza a equalização dos dados recebidos.
 *
 * @param data um ponteiro para o array de dados
 * @param size o tamanho do array de dados
 * @param num_streams o número de streams
 * @param S o vetor S da decomposição SVD
 * @return um ponteiro para o array de dados equalizados
 */
double complex **rx_feq(double complex **data, int size, int num_streams, double *S);

/**
 * Demapeia os símbolos QAM para obter os índices dos dados recebidos.
 *
 * @param data um ponteiro para o array de dados
 * @param size o tamanho do array de dados
 * @return um ponteiro para o array de índices demapeados
 */
int *rx_qam_demapper(double complex *data, int size);

/**
 * Remove o preenchimento dos dados.
 *
 * @param data um ponteiro para o array de dados
 * @param size o tamanho do array de dados
 * @param num_streams o número de streams
 * @param Nt o número de transmissores
 * @return um ponteiro para o array de dados sem preenchimento
 */
double complex *rx_data_depadding(double complex *data, int size, int num_streams, int Nt);

/**
 * Salva os índices dos dados recebidos em um arquivo.
 *
 * @param tx_indices um ponteiro para o array de índices
 * @param size o tamanho do array de índices
 * @param filename o nome do arquivo de saída
 */
void rx_data_write(int *data, int size, char *filename);

/**
 * Gera estatísticas de erro de transmissão.
 *
 * @param tx_indices um ponteiro para o array de índices transmitidos
 * @param rx_indices um ponteiro para o array de índices recebidos
 * @param size o tamanho dos arrays de índices
 */
void gera_estatisticas(int *tx_indices, int *rx_indices, int size);

#endif /* PDS_TELECOM_H */