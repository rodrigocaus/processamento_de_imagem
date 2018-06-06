#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "imageprocessing.h"
#include "filter.h"

#define ORDEM 3
#define NTHREADS 4

int main(int argc, char **argv) {
	float ** emboss;

	if(argc < 3) {
		fprintf(stderr, "Faltam argumentos!\n");
		return argc;
	}
	imagem entrada, saida;
	entrada = abrir_imagem(argv[1]);
	if(entrada.width == 0)
		return 1;

	saida = inicializa_saida(&entrada);
	cria_emboss(&emboss);

	clock_t t0, t1;
	t0 = clock();

	aplica_filtro_threading(&entrada, &saida, (float **)emboss, ORDEM , NTHREADS);
	t1 = clock();
	salvar_imagem(argv[2], &saida);


	printf("%s \t\t", argv[1]);
	printf("%ux%u \t\t", entrada.width, entrada.height);
	printf("%s \t\t", "multithreads");
	printf("%u \t\t", NTHREADS);
	printf("%u \t\t", ORDEM);
	printf("%.2f\n", 1000*(double)(t1-t0)/CLOCKS_PER_SEC);

	liberar_imagem(&entrada);
	liberar_imagem(&saida);
	limpa_filtro(emboss);

	return 0;
}
