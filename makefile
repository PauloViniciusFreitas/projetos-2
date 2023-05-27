# Makefile

# Regra padrão - compila a aplicação toda
aplicacao: biblioteca aplicacao_principal

# Regra para compilar a biblioteca
biblioteca:
	gcc -c matrizes.c -o matrizes.o

# Regra para compilar a aplicação principal
aplicacao_principal:
	gcc main.c matrizes.o -o aplicacao

# Regra para testar a aplicação
teste: aplicacao
	./aplicacao

# Regra para gerar a documentação em formato HTML usando o Doxygen
doc:
	doxygen Doxyfile

# Regra para limpar o repositório
clean:
	rm -f matrizes.o aplicacao
	rm -rf html