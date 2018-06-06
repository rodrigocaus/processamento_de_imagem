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

	// TODO aplicar filtro

	salvar_imagem(argv[2], &saida);
	t1 = clock();

	printf("%s \t\t", argv[1]);
	printf("%ux%u \t\t", entrada.width, entrada.height);
	printf("%s\t\t", "multithreads");
	printf("%u\t\t", NTHREADS);
	printf("%.2f\n", 1000*(double)(t1-t0)/CLOCKS_PER_SEC);

	liberar_imagem(&entrada);
	liberar_imagem(&saida);

	return 0;
}
