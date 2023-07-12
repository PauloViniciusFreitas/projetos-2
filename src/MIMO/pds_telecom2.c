/// @file pds_telecom.c
/// @brief Implementação de Sistema de Comunicação Digital MIMO.

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <gsl/gsl_linalg.h>

/// Lê os índices dos dados a serem transmitidos a partir de um arquivo.

/// @param filename o nome do arquivo
/// @param size um ponteiro para armazenar o tamanho do array
/// @return um ponteiro para o array de inteiros lidos do arquivo

int *tx_data_read(char *filename, int *size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", filename);
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    char *buffer = (char*) malloc(sizeof(char) * file_size);
    fread(buffer, 1, file_size, file);
    fclose(file);

    int *result = (int*) malloc(sizeof(int) * file_size * 4);
    for (int i = 0; i < file_size; i++) {
        for (int j = 3; j >= 0; j--) {
            result[i * 4 + (3 - j)] = (buffer[i] >> (j * 2)) & 0b11;
        }
    }
    free(buffer);
    *size = file_size * 4;
    return result;
}


/// Preenche os dados com zeros para atingir o tamanho desejado, considerando o número de streams e transmissores.

/// @param data um ponteiro para o array de dados
/// @param size o tamanho do array de dados
/// @param num_streams o número de streams
/// @param Nt o número de transmissores
/// @return um ponteiro para o array de dados preenchidos

double complex *tx_data_padding(double complex *data, int size, int num_streams, int Nt) {
    int padded_size = size + (Nt - num_streams);
    double complex *result = malloc(sizeof(double complex) * padded_size);
    for (int i = 0; i < size; i++) {
        result[i] = data[i];
    }
    for (int i = size; i < padded_size; i++) {
        result[i] = 0;
    }
    return result;
}

/// Realiza o mapeamento dos índices para símbolos QAM.

/// @param tx_indices um ponteiro para o array de índices
/// @param size o tamanho do array de índices
/// @return um ponteiro para o array de símbolos QAM

double complex *QAMmapper(int *tx_indices, int size) {
    double complex mapping[] = { -1 + 1*I, -1 - 1*I, 1 + 1*I, 1 - 1*I };
    double complex *result = malloc(sizeof(double complex) * size);
    for (int i = 0; i < size; i++) {
        result[i] = mapping[tx_indices[i]];
    }
    return result;
}

/// Realiza o mapeamento em camada dos dados, dividindo-os em streams.

/// @param data um ponteiro para o array de dados
/// @param size o tamanho do array de dados
/// @param num_streams o número de streams
/// @return um ponteiro para o array de dados mapeados em camadas

double complex **tx_layer_mapper(double complex *data, int size, int num_streams) {
    double complex **result = malloc(sizeof(double complex*) * num_streams);
    for (int i = 0; i < num_streams; i++) {
        result[i] = malloc(sizeof(double complex) * size / num_streams);
    }
    for (int i = 0; i < size; i++) {
        result[i % num_streams][i / num_streams] = data[i];
    }
    return result;
}

/// Gera uma matriz de canal aleatória.

/// @param Nr o número de receptores
/// @param Nt o número de transmissores
/// @return um ponteiro para a matriz de canal gerada

double **channel_gen(int Nr, int Nt) {
    double **H = malloc(sizeof(double*) * Nr);
    for (int i = 0; i < Nr; i++) {
        H[i] = malloc(sizeof(double) * Nt);
        for (int j = 0; j < Nt; j++) {
            H[i][j] = ((double) rand() / RAND_MAX) * 2.0 - 1.0;
        }
    }
    return H;
}

/// Transpõe a matriz de canal.

/// @param H a matriz a ser transposta
/// @param Nr o número de receptores
/// @param Nt o número de transmissores
/// @return um ponteiro para a matriz transposta

double **matrix_transpose(double **H, int Nr, int Nt) {
    double **Ht = malloc(sizeof(double*) * Nt);
    for (int i = 0; i < Nt; i++) {
        Ht[i] = malloc(sizeof(double) * Nr);
        for (int j = 0; j < Nr; j++) {
            Ht[i][j] = H[j][i];
        }
    }
    return Ht;
}

/// Realiza a transmissão dos dados pelo canal, adicionando ruído.

/// @param data um ponteiro para o array de dados
/// @param size o tamanho do array de dados
/// @param num_streams o número de streams
/// @param H a matriz de canal
/// @param Nr o número de receptores
/// @param Nt o número de transmissores
/// @param ruido_min o valor mínimo do ruído
/// @param ruido_max o valor máximo do ruído
/// @return um ponteiro para o array de dados transmitidos

double complex **channel_transmission(double complex **data, int size, int num_streams, double **H, int Hlinhas, int Hcolunas, double ruido_min, double ruido_max) {
    double complex **result = malloc(sizeof(double complex*) * Hlinhas);
    for (int i = 0; i < Hlinhas; i++) {
        result[i] = malloc(sizeof(double complex) * (size/num_streams));
        for (int j = 0; j < Hcolunas; j++) {
            result[i][j] = 0;
            for (int k = 0; k < (size/num_streams); k++) {
                result[i][j] += H[i][k] * data[k][j];
            }
            double ruido_real = ((double) rand() / RAND_MAX) * (ruido_max - ruido_min) + ruido_min;
            double ruido_imaginary = ((double) rand() / RAND_MAX) * (ruido_max - ruido_min) + ruido_min;
            result[i][j] += ruido_real + ruido_imaginary*I;
        }
    }
    return result;
}

/// Realiza a decomposição em valores singulares (SVD) da matriz transposta de canal.

/// @param H a matriz a ser decomposta
/// @param Nr o número de receptores
/// @param Nt o número de transmissores
/// @param U um ponteiro para a matriz U resultante
/// @param S um ponteiro para o vetor S resultante
/// @param V um ponteiro para a matriz V resultante

void svd(double **H, int Nr, int Nt, double **U, double *S, double **V) {
    gsl_matrix *H_gsl = gsl_matrix_alloc(Nr, Nt);
    for (int i = 0; i < Nr; i++) {
        for (int j = 0; j < Nt; j++) {
            gsl_matrix_set(H_gsl, i, j, H[i][j]);
        }
    }

    gsl_matrix *V_gsl = gsl_matrix_alloc(Nt, Nt);
    gsl_vector *S_gsl = gsl_vector_alloc(Nt);
    gsl_vector *work = gsl_vector_alloc(Nt);

    gsl_linalg_SV_decomp(H_gsl, V_gsl, S_gsl, work);

    for (int i = 0; i < Nr; i++) {
        for (int j = 0; j < Nt; j++) {
            U[i][j] = gsl_matrix_get(H_gsl, i, j);
        }
    }

    for (int i = 0; i < Nt; i++) {
        S[i] = gsl_vector_get(S_gsl, i);
        for (int j = 0; j < Nt; j++) {
            V[i][j] = gsl_matrix_get(V_gsl, i, j);
        }
    }
}

/// Pré-codifica os dados utilizando a matriz V resultante da decomposição SVD.

/// @param data um ponteiro para o array de dados
/// @param size o tamanho do array de dados
/// @param num_streams o número de streams
/// @param V a matriz V da decomposição SVD
/// @return um ponteiro para o array de dados pré-codificados

double complex **tx_precoder(double complex **data, int size, int num_streams, double **V, int Vlinhas,int Vcolunas) {
    double complex **result = malloc(sizeof(double complex*) * Vlinhas);
    for (int i = 0; i < Vlinhas; i++){
        result[i] = malloc(sizeof(double complex) * (size / num_streams));
        for (int j = 0; j < (size/num_streams); j++) {
            result[i][j] = 0;
            for (int k = 0; k < Vcolunas; k++) {
                result[i][j] += creal(V[i][k]) * data[k][j];
            }
        }
    }
    return result;
}

/// Combina os dados recebidos utilizando a matriz U resultante da decomposição SVD.

/// @param data um ponteiro para o array de dados
/// @param size o tamanho do array de dados
/// @param num_streams o número de streams
/// @param U a matriz U da decomposição SVD
/// @return um ponteiro para o array de dados combinados

double complex **rx_combiner(double complex **data, int size, int num_streams, double **U, int Ulinhas, int Ucolunas) {
    double complex **result = malloc(sizeof(double complex*) * Ulinhas);
    for (int i = 0; i < Ulinhas; i++) {
        result[i] = malloc(sizeof(double complex) * (size / num_streams));
        for (int j = 0; j < Ucolunas; j++) {
            result[i][j] = 0;
            for (int k = 0; k < num_streams; k++) {
                result[i][j] += creal(U[i][k]) * data[k][j];
            }
        }
    }
    return result;
}

/// Realiza o demapeamento em camada dos dados.

/// @param data um ponteiro para o array de dados
/// @param size o tamanho do array de dados
/// @param num_streams o número de streams
/// @return um ponteiro para o array de dados demapeados em camadas

double complex *rx_layer_demapper(double complex **data, int size, int num_streams) {
    double complex *result = malloc(sizeof(double complex) * size);
    for (int i = 0; i < size; i++) {
        result[i] = data[i % num_streams][i / num_streams];
    }
    return result;
}

/// Realiza a equalização dos dados recebidos.

/// @param data um ponteiro para o array de dados
/// @param size o tamanho do array de dados
/// @param num_streams o número de streams
/// @param S o vetor S da decomposição SVD
/// @return um ponteiro para o array de dados equalizados

double complex **rx_feq(double complex **data, int size, int num_streams, double *S, int Slinhas) {
    double complex **result = malloc(sizeof(double complex*) * Slinhas);
    for (int i = 0; i < Slinhas; i++) {
        result[i] = malloc(sizeof(double complex) * size / num_streams);
        for (int j = 0; j < size / num_streams; j++) {
            result[i][j] = data[i][j] / S[i];
        }
    }
    return result;
}

/// Demapeia os símbolos QAM para obter os índices dos dados recebidos.

/// @param data um ponteiro para o array de dados
/// @param size o tamanho do array de dados
/// @return um ponteiro para o array de índices demapeados

int *rx_qam_demapper(double complex *data, int size) {
    double complex mapping[] = { -1 + 1*I, -1 - 1*I, 1 + 1*I, 1 - 1*I };
    int *result = malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++) {
        double min_distance = INFINITY;
        int min_index = -1;
        for (int j = 0; j < 4; j++) {
            double distance = cabs(data[i] - mapping[j]);
            if (distance < min_distance) {
                min_distance = distance;
                min_index = j;
            }
        }
        result[i] = min_index;
    }
    return result;
}

/// Remove o preenchimento dos dados.

/// @param data um ponteiro para o array de dados
/// @param size o tamanho do array de dados
/// @param num_streams o número de streams
/// @param Nt o número de transmissores
/// @return um ponteiro para o array de dados sem preenchimento

double complex *rx_data_depadding(double complex *data, int size, int num_streams, int Nt) {
    int depadded_size = size - (Nt - num_streams);
    double complex *result = malloc(sizeof(double complex) * depadded_size);
    for (int i = 0; i < depadded_size; i++){
        result[i] = data[i];
    }
    return result;
}

/// Salva os índices dos dados recebidos em um arquivo.

/// @param tx_indices um ponteiro para o array de índices
/// @param size o tamanho do array de índices
/// @param filename o nome do arquivo de saída

void rx_data_write(int *data, int size, char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", filename);
        exit(1);
    }
    char byte = 0;
    for (int i = 0; i < size; i++) {
        byte |= data[i] << ((3 - (i % 4)) * 2);
        if ((i + 1) % 4 == 0) {
            fwrite(&byte, sizeof(char), 1, file);
            byte = 0;
        }
    }
    fclose(file);
}

void gera_estatisticas(int *tx_indices, int *rx_indices, int size) {
    int num_errors = 0;
    for (int i = 0; i < size; i++) {
        if (tx_indices[i] != rx_indices[i]) {
            num_errors++;
        }
    }
    printf("Número de símbolos QAM transmitidos: %d\n", size);
    printf("Número de símbolos QAM recebidos com erro: %d\n", num_errors);
    printf("Porcentagem de símbolos QAM recebidos com erro: %.2f %%\n", (double)num_errors / size * 100);
}

int main(int argc, char *argv[]) {
    int size;
    int num_streams;
    int Nr = 8;
    int Nt = 8;
    if(Nr < Nt){
        num_streams = Nr;
    }else if (Nr >= Nt){
        num_streams = Nt;
    }
    double ruido_min = -0.1;
    double ruido_max = 0.1;

    printf("\n");

    printf("======IMPLEMENTACAO SISTEMA MIMO======");

    printf("\n");

    printf("'Equipe':Filipe Correa da silva");

    printf("\n");

    if (argc != 3) {
        printf("Uso: %s <arquivo de entrada> <arquivo de saída>\n", argv[0]);
        exit(1);
    }
    
    int *tx_indices = tx_data_read(argv[1], &size);

    printf("\n");

    printf("Indices: [");
    for (int i = 0; i < size; i++) {
        printf("%d", tx_indices[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    printf("\n");

    double complex *data = QAMmapper(tx_indices, size);

    double complex *padded_data = tx_data_padding(data, size, num_streams, Nt);

    printf("Simbolos QAM: [");
    for (int i = 0; i < size + (Nt - num_streams); i++) {
        printf("%.0f%+.0fj", creal(padded_data[i]), cimag(padded_data[i]));
        if (i < size + (Nt - num_streams) - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    printf("\n");

    double complex **result = tx_layer_mapper(padded_data, size, num_streams);

    for (int i = 0; i < num_streams; i++) {
        printf("Stream %d: [", i);
        for (int j = 0; j < size / num_streams; j++) {
            printf("%.0f%+.0fj", creal(result[i][j]), cimag(result[i][j]));
            if (j < size / num_streams - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }

    printf("\n");

    double **H = channel_gen(Nr, Nt);

    double **Ht = matrix_transpose(H, Nr, Nt);

    printf("Matriz H transposta:\n");
    for (int i = 0; i < Nt; i++) {
        for (int j = 0; j < Nr; j++) {
            printf("%f ", Ht[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    double **U = malloc(sizeof(double*) * Nr);
    for (int i = 0; i < Nr; i++) {
        U[i] = malloc(sizeof(double) * Nr);
    }

    double *S = malloc(sizeof(double) * Nr);

    double **V = malloc(sizeof(double*) * Nt);
    for (int i = 0; i < Nt; i++) {
        V[i] = malloc(sizeof(double) * Nr);
    }
    
    svd(Ht, Nt, Nr, V, S, U);

    printf("Matriz U:\n");
    for (int i = 0; i < Nr; i++) {
        for (int j = 0; j < Nr; j++) {
            printf("%f ", U[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    printf("Vetor S:\n[");
    for (int i = 0; i < Nr; i++) {
        printf("%f", S[i]);
        if (i < Nr - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    printf("\n");

    printf("Matriz V:\n");
    for (int i = 0; i < Nt; i++) {
        for (int j = 0; j < Nr; j++) {
            printf("%f ", V[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    double complex **precoded_data = tx_precoder(result, size, num_streams, V, Nt, Nr);

    printf("Dados pre-codificados:\n");
    for (int i = 0; i < Nt; i++) {
        printf("Stream %d: [", i);
        for (int j = 0; j < size / num_streams; j++) {
            printf("%.2f%+.2fj", creal(precoded_data[i][j]), cimag(precoded_data[i][j]));
            if (j < size / num_streams - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }

    printf("\n");

    double complex **canal_data = channel_transmission(precoded_data, size, num_streams, H, Nr, Nt, ruido_min, ruido_max);

    printf("Dados transmitidos:\n");
    for (int i = 0; i < Nr; i++) {
        printf("Stream %d: [", i);
        for (int j = 0; j < size / num_streams; j++) {
            printf("%.2f%+.2fj", creal(canal_data[i][j]), cimag(canal_data[i][j]));
            if (j < size / num_streams - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }

    printf("\n");

    double ** Ut = matrix_transpose(U, Nr, Nr);
    
    double complex **combined_data = rx_combiner(canal_data, size, num_streams, U, Nr, Nr);

    printf("Dados combinados:\n");
    for (int i = 0; i < num_streams; i++) {
        printf("Stream %d: [", i);
        for (int j = 0; j < size / num_streams; j++) {
            printf("%.2f%+.2fj", creal(combined_data[i][j]), cimag(combined_data[i][j]));
            if (j < size / num_streams - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }

    printf("\n");

    double complex **equalized_data = rx_feq(combined_data, size, num_streams, S, Nr);

    printf("Dados equalizados:\n");
    for (int i = 0; i < num_streams; i++) {
        printf("Stream %d: [", i);
        for (int j = 0; j < size / num_streams; j++) {
            printf("%.2f%+.2fj", creal(equalized_data[i][j]), cimag(equalized_data[i][j]));
            if (j < size / num_streams - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }

    printf("\n");

    double complex *rx_demapper = rx_layer_demapper(equalized_data, size, num_streams);

    printf("Dados recuperados: [");
    for (int i = 0; i < size; i++) {
        printf("%.3f%+.3fj", creal(rx_demapper[i]), cimag(rx_demapper[i]));
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    printf("\n");

    int *rx_indices = rx_qam_demapper(rx_demapper, size);

    printf("Indices recuperados: [");
    for (int i = 0; i < size; i++) {
        printf("%d", rx_indices[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    printf("\n");

    double complex *depadded_data = rx_data_depadding(data, size, num_streams, Nt);

    printf("Dados preenchidos: [");
    for (int i = 0; i < size; i++) {
        printf("%.0f%+.0fj", creal(data[i]), cimag(data[i]));
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    printf("\n");

    printf("Dados sem preenchimento: [");
    for (int i = 0; i < size - (Nt - num_streams); i++) {
        printf("%.0f%+.0fj", creal(depadded_data[i]), cimag(depadded_data[i]));
        if (i < size - (Nt - num_streams) - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    printf("\n");

    gera_estatisticas(tx_indices, rx_indices, size);

    rx_data_write(rx_indices, size, argv[2]);

    for (int i = 0; i < Nr; i++) {
        free(H[i]);
        free(U[i]);
        free(canal_data[i]);
    }
    free(H);
    free(U);
    free(S);
    free(canal_data);

    for (int i = 0; i < num_streams; i++) {
        free(result[i]);
        free(precoded_data[i]);
        free(combined_data[i]);
        free(equalized_data[i]);
    }
    free(result);
    free(precoded_data);
    free(combined_data);
    free(equalized_data);
    
    for (int i = 0; i < Nt; i++) {
        free(Ht[i]);
        free(Ut[i]);
        free(V[i]);
    }
    free(H);
    free(Ut);
    free(V);

    free(tx_indices);
    free(data);
    free(padded_data);
    free(rx_indices);
    free(rx_demapper);
    free(depadded_data);

    return 0;
}