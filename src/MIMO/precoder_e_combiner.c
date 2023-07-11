#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <gsl/gsl_linalg.h>

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

double complex **equalize(double complex **data, int size, int num_streams, double *S) {
    double complex **result = malloc(sizeof(double complex*) * num_streams);
    for (int i = 0; i < num_streams; i++) {
        result[i] = malloc(sizeof(double complex) * size / num_streams);
        for (int j = 0; j < size / num_streams; j++) {
            result[i][j] = data[i][j] / S[i];
        }
    }
    return result;
}

double complex **tx_precoder(double complex **data, int size, int num_streams, double **V) {
    double complex **result = malloc(sizeof(double complex*) * num_streams);
    for (int i = 0; i < num_streams; i++) {
        result[i] = malloc(sizeof(double complex) * size / num_streams);
        for (int j = 0; j < size / num_streams; j++) {
            result[i][j] = 0;
            for (int k = 0; k < num_streams; k++) {
                result[i][j] += conj(V[k][i]) * data[k][j];
            }
        }
    }
    return result;
}

double complex **rx_combiner(double complex **data, int size, int num_streams, double **U) {
    double complex **result = malloc(sizeof(double complex*) * num_streams);
    for (int i = 0; i < num_streams; i++) {
        result[i] = malloc(sizeof(double complex) * size / num_streams);
        for (int j = 0; j < size / num_streams; j++) {
            result[i][j] = 0;
            for (int k = 0; k < num_streams; k++) {
                result[i][j] += conj(U[k][i]) * data[k][j];
            }
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

    printf("\n");

    printf("Indices: [");
    for (int i = 0; i < size; i++) {
        printf("%d", indices[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    printf("\n");

    double complex *data = QAMmapper(indices, size);

    printf("Simbolos QAM: [");
    for (int i = 0; i < size; i++) {
        printf("%.0f%+.0fj", creal(data[i]), cimag(data[i]));
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    printf("\n");

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

    printf("\n");

    int Nr = 2;
    int Nt = 2;
    double **H = channel_gen(Nr, Nt);
    double **U = malloc(sizeof(double*) * Nr);
    for (int i = 0; i < Nr; i++) {
        U[i] = malloc(sizeof(double) * Nt);
    }

    double *S = malloc(sizeof(double) * Nt);

    double **V = malloc(sizeof(double*) * Nt);
    for (int i = 0; i < Nt; i++) {
        V[i] = malloc(sizeof(double) * Nt);
    }

    double noise_min = -1.0;
    double noise_max = 1.0;
    double complex **transmitted_data = channel_transmission(result, size, num_streams, H, Nr, Nt, noise_min, noise_max);

    printf("Matriz H:\n");
    for (int i = 0; i < Nr; i++) {
        for (int j = 0; j < Nt; j++) {
            printf("%f ", H[i][j]);
        }
        printf("\n");
    }

    printf("\n");

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

    printf("\n");

    printf("Calculo svd da matriz H:\n");
    
    printf("\n");

    svd(H, Nr, Nt, U, S, V);

    printf("Matriz U:\n");
    for (int i = 0; i < Nr; i++) {
        for (int j = 0; j < Nt; j++) {
            printf("%f ", U[i][j]);
        }
        printf("\n");
    }

    printf("\n");

        printf("Vetor S:\n[");
    for (int i = 0; i < Nt; i++) {
        printf("%f", S[i]);
        if (i < Nt - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    printf("\n");

    printf("Matriz V:\n");
    for (int i = 0; i < Nt; i++) {
        for (int j = 0; j < Nt; j++) {
            printf("%f ", V[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    double complex **precoded_data = tx_precoder(result, size, num_streams, V);

    printf("Dados pre-codificados:\n");
    for (int i = 0; i < num_streams; i++) {
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

    double complex **combined_data = rx_combiner(transmitted_data, size, num_streams, U);

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

    double complex **equalized_data = equalize(combined_data, size, num_streams, S);

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

    for (int i = 0; i < Nr; i++) {
        free(H[i]);
        free(transmitted_data[i]);
    }
    free(H);
    free(transmitted_data);

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

    for (int i = 0; i < Nr; i++) {
        free(U[i]);
    }
    free(U);

    free(S);

    for (int i = 0; i < Nt; i++) {
        free(V[i]);
    }
    free(V);

    free(indices);
    free(data);
    
    return 0;
}