#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "imageprocessing.h"
#include "filter.h"

#define ORDEM 3
#define NTHREADS 1

int main(int argc, char **argv) {
	
	struct timespec t1, t2;
	double duracao;

	if(argc < 3) {
		fprintf(stderr, "Faltam argumentos!\n");
		return argc;
	}
	imagem entrada, saida;
	entrada = abrir_imagem(argv[1]);
	if(entrada.width == 0)
		return 1;

	saida = inicializa_saida(&entrada);

	clock_gettime(CLOCK_MONOTONIC, &t1);
	// TODO aplicar o filtro
	clock_gettime(CLOCK_MONOTONIC, &t2);

	salvar_imagem(argv[2], &saida);

	duracao = (t2.tv_sec - t1.tv_sec);
	duracao += (t2.tv_nsec - t1.tv_nsec) / 1000000000.0;

	printf("%s \t\t", argv[1]);
	printf("%ux%u \t\t", entrada.width, entrada.height);
	printf("%s \t\t", "multiprocessos");
	printf("%u \t\t", NTHREADS);
	printf("%u \t\t", ORDEM);
	printf("%f\n", duracao);

	liberar_imagem(&entrada);
	liberar_imagem(&saida);

	return 0;
}
