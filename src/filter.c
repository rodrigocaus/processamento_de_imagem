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
	
    //Temp Red, Temp Green e Temp Blue : Acumuladores usados durante a convolucao
    float tr, tg, tb;

    //Amplitude em pixels em relação ao atual em que a matriz de convolução tem efeito
    int amp = (int)(ordem/2);

    //Percorre as linhas da imagem
    for(int y = 0 ; y < (I->height) ; y++)
    {
        //Percorre as colunas da imagem
        for(int x = 0 ; x < (I->width) ; x++)
        {

            tr = 0;
            tg = 0;
            tb = 0;

            //Inicia o valor da convolucao para o pixel [y][x] atual
            //"mc" e "nc" índices da matriz que estao sendo multiplicados na conv
         
            //Percorre as linhas da matriz de convolucao
            for(int m = 0 ; m < ordem ; m++)
            {

                //Não é caso de borda
                int mc = y - amp + m;

                //Fora do limite superior
                if(mc >= (I->height)) mc = ((I->height)-1);

                //Fora do limite inferior
                if(mc < 0) mc = 0;
                
                //Percorre as colunas da matriz de convolucao
                for(int n = 0 ; n < ordem ; n++)
                {

                    //Não é caso de borda
                    int nc = x - amp + n;

                    //Fora do limite direito
                    if(nc >= (I->width)) nc = ((I->width)-1);

                    //Fora do limite esquerdo
                    if(nc < 0) nc = 0;

                    //Realiza o produto entre o pixel tratado no momento da imagem e da matriz de conv
                    //Para cada uma das cores
                    tr = tr + ((I->r)[mc][nc] * filtro[m][n]);
                    tg = tg + ((I->g)[mc][nc] * filtro[m][n]);
                    tb = tb + ((I->b)[mc][nc] * filtro[m][n]);
                }
            }

            //Verifica overflow
            if(tr > 255) tr = 255;
            if(tg > 255) tg = 255;
            if(tb > 255) tb = 255;

            //Verifica negativo
            if(tr < 0) tr = 0;
            if(tg < 0) tg = 0;
            if(tb < 0) tb = 0;
           
            //Escreve na imagem de saida o valor calculado para o pixel
            //Em cada uma das cores
            (O->r)[y][x] = tr;
            (O->g)[y][x] = tg;
            (O->b)[y][x] = tb;
        }
    }

	return;
}
