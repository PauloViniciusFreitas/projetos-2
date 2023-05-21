#cria os arquivos .o:
#gcc -c main.c -o main.o
#gcc -c matrizes.c -o matrizes.o
#executa:
#./matrizes

all: matrizes

matrizes: matrizes.o main.o
	gcc -o matrizes matrizes.o main.o

matrizes.o: matrizes.c matrizes.h
	gcc -o matrizes.o -c matrizes.c -W -Wall -pedantic

main.o: main.c matrizes.h
	gcc -o main.o -c main.c -W -Wall -pedantic

clean:
	rm -f matrizes.o main.o matrizes