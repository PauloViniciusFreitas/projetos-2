#include <stdio.h>
#include <stdlib.h>
#include <complex.h>

int *tx_data_read(char *filename, int *size) {
    FILE *file = fopen(filename, "a");
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

    free(indices);
    free(data);
    return 0;
}
