#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>


#include "imageprocessing.h"
#include "filter.h"


void *shalloc(size_t len) {
	//Aloca mapa de memória anônima compartilhada
	return mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
}

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

imagem inicializa_saida_shared(imagem *I) {
	imagem saida;
	int x, y;

	saida.width = x = I->width; 	//Numero de colunas
	saida.height = y = I->height;	//Numero de linhas

    //A alocação da matriz é feita dinamicamente
    //Sao apenas dois malloc's:
    //    1 alocação das linhas da matriz
    //    outra alocação para todas as colunas para a primeira linha

    saida.r = (float **) shalloc(sizeof(float *) * y);
    saida.r[0] = (float *) shalloc(sizeof(float) * y * x);

    saida.g = (float **) shalloc(sizeof(float *) * y);
    saida.g[0] = (float *) shalloc(sizeof(float) * y * x);

    saida.b = (float **) shalloc(sizeof(float *) * y);
    saida.b[0] = (float *) shalloc(sizeof(float) * y * x);

    //A cada linha fica atribuida uma seção de colunas
    for(int i = 0; i < y; i++) {
        saida.r[i] = (*(saida.r) + x*i);
        saida.g[i] = (*(saida.g) + x*i);
        saida.b[i] = (*(saida.b) + x*i);
    }

	return saida;
}

void liberar_imagem_shared(imagem *I) {

	int x = I->width; 	//Numero de colunas
	int y = I->height;	//Numero de linhas

	munmap(I->r[0], sizeof(float) * y * x);
	munmap(I->g[0], sizeof(float) * y * x);
	munmap(I->b[0], sizeof(float) * y * x);
	munmap(I->r, sizeof(float *) * y);
	munmap(I->g, sizeof(float *) * y);
	munmap(I->b, sizeof(float *) * y);

}

void aplica_filtro_single(imagem *I, imagem *O, float **filtro, int ordem) {


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

void *thread_worker(void * args)
{
    //Salva localmente os valores recebidos por argumento
	int l_ini = ((area_trab *) args)->l_ini;
	int l_fim = ((area_trab *) args)->l_fim;
	int ordem = ((area_trab *) args)->ordem;
	imagem * I = ((area_trab *) args)->I;
	imagem * O = ((area_trab *) args)->O;
	float ** filtro = ((area_trab *) args)->filtro;

	//Temp Red, Temp Green e Temp Blue : Acumuladores usados durante a convolucao
    float tr, tg, tb;

    //Amplitude em pixels em relação ao atual em que a matriz de convolução tem efeito
    int amp = (int)(ordem/2);

    //Percorre as linhas da imagem apenas nas linhas atribuidas a esse worker
    for(int y = l_ini ; y <= l_fim ; y++) {
        //Percorre as colunas da imagem
        for(int x = 0 ; x < (I->width) ; x++) {
            tr = 0;
            tg = 0;
            tb = 0;
            //Inicia o valor da convolucao para o pixel [y][x] atual
            //"mc" e "nc" índices da matriz que estao sendo multiplicados na conv

            //Percorre as linhas da matriz de convolucao
            for(int m = 0 ; m < ordem ; m++) {
                //Não é caso de borda
                int mc = y - amp + m;
                //Fora do limite superior
                if(mc >= (I->height)) mc = ((I->height)-1);
                //Fora do limite inferior
                if(mc < 0) mc = 0;

                //Percorre as colunas da matriz de convolucao
                for(int n = 0 ; n < ordem ; n++) {
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


}


void aplica_filtro_threading(imagem *I, imagem *O, float **filtro, int ordem, int n_thread) {

    //Aloca o vetor de threads
   	pthread_t * threads = (pthread_t *) malloc(sizeof(pthread_t) * n_thread);

   	//Aloca o vetor de structs de argumentos para as threads
   	area_trab * args = malloc(sizeof(area_trab) * n_thread);

   	int n_linhas = I->height;
   	int linha_ant = -1;
   	int div = n_linhas/n_thread;

    //Realiza a divisao das linhas entre as threads
    //Faz as chamadas dos workers
    for(int i=0 ; i < n_thread ; i++)
    {
    	//Define os as linhas limite de trabalho para as threads
    	//Linha inicial eh a proxima apos o fim da area de trabalho anterior
    	(args[i]).l_ini = linha_ant + 1;

    	//Se for a ultima thread, pega ate o fim
    	//Se nao for, pega ate o multiplo de divisao
    	if(i == n_thread-1) {(args[i]).l_fim = n_linhas - 1;}
    	else {(args[i]).l_fim = (i+1) * div;}

    	//Atualiza a linha anterior
    	linha_ant = (args[i]).l_fim ;

    	(args[i]).ordem = ordem;
    	(args[i]).I = I;
    	(args[i]).O = O;
    	(args[i]).filtro = filtro;

    	//Cria a thread
    	pthread_create(&threads[i], NULL, thread_worker, &args[i]);
    }

    // Esperando threads
  	for (int i = 0; i < n_thread; i++)
  	{
    	pthread_join(threads[i], NULL);
	}

   	//Libera o vetor de threads e a struct de argumentos
   	free(threads);
   	free(args);

    return;
}

void aplica_filtro_process(imagem *I, imagem *O, float **filtro, int ordem, int n_proc) {

	//Aloca o vetor de processos
   	pid_t *processos = (pid_t *) malloc(sizeof(pid_t) * n_proc);

   	int n_linhas = I->height;
   	int linha_ant = -1;
   	int div = n_linhas/n_proc;

    //Realiza a divisao das linhas entre os processos
    //Faz as chamadas dos workers
    for(int i=0 ; i < n_proc; i++)
    {
		int l_ini, l_fim;
    	//Define os as linhas limite de trabalho para as threads
    	//Linha inicial eh a proxima apos o fim da area de trabalho anterior
    	l_ini = linha_ant + 1;

    	//Se for a ultima thread, pega ate o fim
    	//Se nao for, pega ate o multiplo de divisao
    	if(i == n_proc-1) {l_fim = n_linhas - 1;}
    	else {l_fim = (i+1) * div;}

    	//Atualiza a linha anterior
    	linha_ant = l_fim ;

		//Cria-se o processo
		processos[i] = fork();
		if(processos[i] == 0) {
			//Essa parte executa no processo filho

			//Temp Red, Temp Green e Temp Blue : Acumuladores usados durante a convolucao
		    float tr, tg, tb;
		    //Amplitude em pixels em relação ao atual em que a matriz de convolução tem efeito
		    int amp = (int)(ordem/2);

		    //Percorre as linhas da imagem apenas nas linhas atribuidas a esse worker
		    for(int y = l_ini ; y <= l_fim ; y++) {
		        //Percorre as colunas da imagem
		        for(int x = 0 ; x < (I->width) ; x++) {
		            tr = 0;
		            tg = 0;
		            tb = 0;
		            //Inicia o valor da convolucao para o pixel [y][x] atual
		            //"mc" e "nc" índices da matriz que estao sendo multiplicados na conv

		            //Percorre as linhas da matriz de convolucao
		            for(int m = 0 ; m < ordem ; m++) {
		                //Não é caso de borda
		                int mc = y - amp + m;
		                //Fora do limite superior
		                if(mc >= (I->height)) mc = ((I->height)-1);
		                //Fora do limite inferior
		                if(mc < 0) mc = 0;

		                //Percorre as colunas da matriz de convolucao
		                for(int n = 0 ; n < ordem ; n++) {
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

			exit(0);
		}
    }

    // Esperando os processos
  	for (int i = 0; i < n_proc; i++)
  	{
		waitpid(processos[i], NULL, 0);
	}

   	//Libera o vetor de processos
   	free(processos);

    return;
}

void cria_blur(float ***filtro, int ordem) {

    (*filtro) = malloc(sizeof(float *) * ordem);
	(*filtro)[0] = malloc(sizeof(float) * ordem * ordem);

	for(int i = 0; i < ordem; i++)
		(*filtro)[i] = (*(*filtro) + ordem*i);

	for(int i = 0; i < ordem; i++)
		for(int j = 0; j < ordem; j++)
			(*filtro)[i][j] = 1.0/(float)(ordem*ordem);
}

void cria_motion_blur(float ***filtro, int ordem) {

    (*filtro) = malloc(sizeof(float *) * ordem);
    (*filtro)[0] = malloc(sizeof(float) * ordem * ordem);

    for(int i = 0; i < ordem; i++)
        (*filtro)[i] = (*(*filtro) + ordem*i);

    for(int i = 0; i < ordem; i++)
        for(int j = 0; j < ordem; j++)
            if(i==j) {(*filtro)[i][j] = 1.0/(float)(ordem);}
            else { (*filtro)[i][j] = 0.0;}
}


void cria_emboss(float ***filtro) {
	//Apenas 3X3

	(*filtro) = malloc(sizeof(float *) * 3);
	(*filtro)[0] = malloc(sizeof(float) * 9);
	for(int i = 0; i < 3; i++)
		(*filtro)[i] = (*(*filtro) + 3*i);

	(*filtro)[0][0] = -2; (*filtro)[0][1] = -2; (*filtro)[0][2] = 0;
    (*filtro)[1][0] = -2; (*filtro)[1][1] = 2;  (*filtro)[1][2] = 1;
    (*filtro)[2][0] = 0;  (*filtro)[2][1] = 1;  (*filtro)[2][2] = 2;

}

void cria_emboss_5(float ***filtro) {
    //Apenas 3X3

    (*filtro) = malloc(sizeof(float *) * 5);
    (*filtro)[0] = malloc(sizeof(float) * 25);
    for(int i = 0; i < 5; i++)
        (*filtro)[i] = (*(*filtro) + 5*i);

    (*filtro)[0][0] = -3; (*filtro)[0][1] = -2; (*filtro)[0][2] = -1; (*filtro)[0][3] = -1; (*filtro)[0][4] = 0;

    (*filtro)[1][0] = -2; (*filtro)[1][1] = -1;  (*filtro)[1][2] = -1; (*filtro)[1][3] = 0; (*filtro)[1][4] = 1;

    (*filtro)[2][0] = -1;  (*filtro)[2][1] = -1;  (*filtro)[2][2] = 1; (*filtro)[2][3] = 1; (*filtro)[2][4] = 1;

    (*filtro)[3][0] = -1;  (*filtro)[3][1] = 0;  (*filtro)[3][2] = 1; (*filtro)[3][3] = 1; (*filtro)[3][4] = 2;

    (*filtro)[4][0] = 0;  (*filtro)[4][1] = 1;  (*filtro)[4][2] = 1; (*filtro)[4][3] = 2; (*filtro)[4][4] = 3;

}

void cria_edge_detection(float ***filtro, int ordem) {
	(*filtro) = malloc(sizeof(float *) * ordem);
	(*filtro)[0] = malloc(sizeof(float) * ordem * ordem);
	for(int i = 0; i < ordem; i++)
		(*filtro)[i] = (*(*filtro) + ordem*i);

	for(int i = 0; i < ordem; i++) {
		for(int j = 0; j < ordem; j++) {
			if(i == ordem/2 && j == ordem/2)
				(*filtro)[i][j] = (float)(ordem*ordem);
			else
				(*filtro)[i][j] = -1.0;
		}
	}
}

void limpa_filtro(float **filtro) {
	free(filtro[0]);
	free(filtro);
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
