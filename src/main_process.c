#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "imageprocessing.h"
#include "filter.h"

#define ORDEM 3
#define NTHREADS 1

float blur[ORDEM][ORDEM] = {1.0/9.0, 1.0/9.0, 1.0/9.0,
							1.0/9.0, 1.0/9.0, 1.0/9.0,
							1.0/9.0, 1.0/9.0, 1.0/9.0};

int main(int argc, char **argv) {
	if(argc < 3) {
		fprintf(stderr, "Faltam argumentos!\n");
		return argc;
	}
	imagem entrada, saida;
	entrada = abrir_imagem(argv[1]);
	if(entrada.width == 0)
		return 1;

	saida = inicializa_saida(&entrada);

	clock_t t0, t1;
	t0 = clock();

	// TODO aplicar o filtro

	salvar_imagem(argv[2], &saida);
	t1 = clock();

	printf("%s\t", argv[1]);
	printf("%ux%u\t", entrada.width, entrada.height);
	printf("%s\t", "Simples");
	printf("%u\t", NTHREADS);
	printf("%.5f\n", 1000*(double)(t1-t0)/CLOCKS_PER_SEC);

	return 0;
}
