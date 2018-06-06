#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "imageprocessing.h"
#include "filter.h"

#define ORDEM 3
#define NTHREADS 1

void copia_imagem(imagem *i, imagem *o);

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
	//aplica_filtro_single(&entrada, &saida, (float **)blur, ORDEM);
	copia_imagem(&entrada, &saida);
	salvar_imagem(argv[2], &saida);
	t1 = clock();

	printf("%s \t\t", argv[1]);
	printf("%ux%u \t\t", entrada.width, entrada.height);
	printf("%s\t\t", "single thread");
	printf("%u\t\t", NTHREADS);
	printf("%.2f\n", 1000*(double)(t1-t0)/CLOCKS_PER_SEC);

	liberar_imagem(&entrada);
	liberar_imagem(&saida);

	return 0;
}

void copia_imagem(imagem *I, imagem *O) {
	for(int i = 0; i < I->height; i++) {
		for(int j = 0; j < I->width; j++) {
			O->r[i][j] = I->r[i][j];
			O->g[i][j] = I->g[i][j];
			O->b[i][j] = I->b[i][j];
		}
	}
}
