# Makefile

# Regra padrão - compila a aplicação toda
aplicacao: biblioteca aplicacao_principal

# Regra para compilar a biblioteca
biblioteca:
	mkdir -p build
	gcc -c -I"/usr/include/" src/matrizes/matrizes.c -o build/matrizes.o

# Regra para compilar a aplicação principal
aplicacao_principal:  biblioteca
	mkdir -p build
	gcc src/matrizes/main.c build/matrizes.o -L"/usr/lib/x86_64-linux-gnu/" -lgsl -lm -o build/aplicacao
	gcc src/MIMO/pds_telecom.c -L"/usr/lib/x86_64-linux-gnu/" -lgsl -lm -o build/pds_telecom

# Regra para testar a aplicação
teste: aplicacao
	./build/aplicacao
	./build/pds_telecom entrada.txt saida.bin

# Regra para gerar a documentação em formato HTML usando o Doxygen
doc:
	mkdir -p doc
	doxygen Doxyfile

# Regra para limpar o repositório
clean:
	rm -rf doc build
