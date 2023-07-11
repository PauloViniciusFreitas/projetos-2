#include <stdio.h>
#include <stdlib.h>
#include <complex.h>

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

double complex *QAMmapper(int *indices, int size) {
    double complex mapping[] = { -1 + 1*I, -1 - 1*I, 1 + 1*I, 1 - 1*I };
    double complex *result = malloc(sizeof(double complex) * size);
    for (int i = 0; i < size; i++) {
        result[i] = mapping[indices[i]];
    }
    return result;
}

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

double complex **channel_transmission(double complex **data, int size, int num_streams, double **H, int Nr, int Nt, double noise_min, double noise_max) {
    double complex **result = malloc(sizeof(double complex*) * Nr);
    for (int i = 0; i < Nr; i++) {
        result[i] = malloc(sizeof(double complex) * size / num_streams);
        for (int j = 0; j < size / num_streams; j++) {
            result[i][j] = 0;
            for (int k = 0; k < Nt; k++) {
                result[i][j] += data[k][j] * H[i][k];
            }
            double noise_real = ((double) rand() / RAND_MAX) * (noise_max - noise_min) + noise_min;
            double noise_imaginary = ((double) rand() / RAND_MAX) * (noise_max - noise_min) + noise_min;
            result[i][j] += noise_real + noise_imaginary*I;
        }
    }
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo de entrada> <arquivo de saída>\n", argv[0]);
        exit(1);
    }
    int size;
    int *indices = tx_data_read(argv[1], &size);

    printf("Indices: [");
    for (int i = 0; i < size; i++) {
        printf("%d", indices[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    double complex *data = QAMmapper(indices, size);

    printf("Simbolos QAM: [");
    for (int i = 0; i < size; i++) {
        printf("%.0f%+.0fj", creal(data[i]), cimag(data[i]));
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    int num_streams = 2;
    double complex **result = tx_layer_mapper(data, size, num_streams);

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

    int Nr = 2;
    int Nt = 2;
    double **H = channel_gen(Nr, Nt);
    
    printf("Matriz H:\n");
    for (int i = 0; i < Nr; i++) {
        for (int j = 0; j < Nt; j++) {
            printf("%f ", H[i][j]);
        }
        printf("\n");
    }

    double noise_min = -1.0;
    double noise_max = 1.0;
    double complex **transmitted_data = channel_transmission(result, size, num_streams, H, Nr, Nt, noise_min, noise_max);

    printf("Dados transmitidos:\n");
    for (int i = 0; i < Nr; i++) {
        printf("Stream %d: [", i);
        for (int j = 0; j < size / num_streams; j++) {
            printf("%.2f%+.2fj", creal(transmitted_data[i][j]), cimag(transmitted_data[i][j]));
            if (j < size / num_streams - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }

    for (int i = 0; i < Nr; i++) {
        free(H[i]);
        free(transmitted_data[i]);
    }
    free(H);
    free(transmitted_data);

    for (int i = 0; i < num_streams; i++) {
        free(result[i]);
    }
    free(result);

    free(indices);
    free(data);
    
    return 0;
}