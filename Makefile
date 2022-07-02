all: codificacao.c conversao_nauty.c gerar_instancia_positiva.c
	gcc -o codificacao codificacao.c
	gcc -o conversao_nauty conversao_nauty.c
	gcc -o gerar_instancia_positiva gerar_instancia_positiva.c
