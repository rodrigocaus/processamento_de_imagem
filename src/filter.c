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

    //Percorre as colunas da imagem
    for(int y = 0 ; y < (I->height) ; y++)
    {
        //Percorre as linhas da imagem
        for(int x = 0 ; x < (I->width) ; x++)
        {

            tr = 0;
            tg = 0;
            tb = 0;

            //Inicia o valor da convolucao para o pixel [y][x] atual
            //"me" e "ne" índices esperados dos pixels de conv
            //"mv" e "nv" índices verdadeiros dos pixels de conv
            //Para repetir pixels na borda quando sai dos limites

            for(int me = y-amp ; me <= y+amp ; me++)
            {

                //Não é caso de borda
                int mv = me;

                //Fora do limite superior
                if(me >= (I->height)) mv = ((I->height)-1);

                //Fora do limite inferior
                if(me < 0) mv = 0;
                

                for(int ne = x-amp ; ne <= x+amp ; ne++)
                {

                    //Não é caso de borda
                    int nv = ne;

                    //Fora do limite direito
                    if(ne >= (I->width)) nv = ((I->width)-1);

                    //Fora do limite esquerdo
                    if(ne < 0) nv = 0;

                    //Realiza o produto entre o pixel tratado no momento da imagem e da matriz de conv

                    tr = tr + ((I->r)[mv][nv] * filtro[][]);
                    

                }
            }

            (O->r)[y][x] = tr;
            (O->g)[y][x] = tg;
            (O->b)[y][x] = tb;
        }
    }
	


	return;
}
