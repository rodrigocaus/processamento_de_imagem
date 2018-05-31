#include <stdio.h>
#include <stdlib.h>

#include "imageprocessing.h"
#include "filter.h"


imagem inicializa_saida(imagem *I) {
	imagem saida;
	int x, y;

	saida.width = x = I->width; 	//Numero de colunas
	saida.height = y = I->height;	//Numero de linhas

    //A alocação da matriz é feita dinamicamente
    //Sao apenas dois malloc's:
    //    1 alocação das linhas da matriz
    //    outra alocação para todas as colunas para a primeira linha

    saida.r = (float **) malloc(sizeof(float *) * y);
    saida.r[0] = (float *) malloc(sizeof(float) * y * x);

    saida.g = (float **) malloc(sizeof(float *) * y);
    saida.g[0] = (float *) malloc(sizeof(float) * y * x);

    saida.b = (float **) malloc(sizeof(float *) * y);
    saida.b[0] = (float *) malloc(sizeof(float) * y * x);

    //A cada linha fica atribuida uma seção de colunas
    for(int i = 0; i < y; i++) {
        saida.r[i] = (*(saida.r) + x*i);
        saida.g[i] = (*(saida.g) + x*i);
        saida.b[i] = (*(saida.b) + x*i);
    }

	return saida;
}

void aplica_filtro(imagem *I, imagem *O, float **filtro, int ordem) {
	// TODO
	for(int i = 0; i < 1000000; i++);
	return;
}
