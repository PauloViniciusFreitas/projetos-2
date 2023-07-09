#include <stdio.h>
#include <stdlib.h>

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
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo de entrada> <arquivo de saída>\n", argv[0]);
        exit(1);
    }
    int size;
    int *data = tx_data_read(argv[1], &size);
    rx_data_write(data, size, argv[2]);
    free(data);
    return 0;
}