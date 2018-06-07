#ifndef FILTER_H
#define FILTER_H

//Struct onde são passados os argumentos para os threads
typedef struct {
    unsigned int l_ini;
    unsigned int l_fim;
    int ordem;
    imagem * I;
    imagem * O;
    float ** filtro;
} area_trab;

//Faz a alocação dinamica com mallocs de uma struct imagem e seus componentes
imagem inicializa_saida(imagem *I);

//Faz a alocação dinamica com mmap (memoria compartilhada) de uma struct imagem e seus componentes
imagem inicializa_saida_shared(imagem *I);

//Realiza o unmap da struct imagem e seus componentes
void liberar_imagem_shared(imagem *I);

//Funcoes que recebem os ponteiros para struct de imagem de entrada e saida,
//o filtro que sera utilizado na convolucao e sua ordem. No caso da funcao
//de threads e processos, recebe tambem o numero de workers dedejados
void aplica_filtro_single(imagem *I, imagem *O, float **filtro, int ordem);
void aplica_filtro_threading(imagem *I, imagem *O, float **filtro, int ordem, int n_thread);
void aplica_filtro_process(imagem *I, imagem *O, float **filtro, int ordem, int n_proc);

//Funcoes que recebem um ponteiro para matriz, alocam a memoria
//necessaria para ela e criam um filtro na ordem desejada
void cria_blur(float ***filtro, int ordem);
void cria_emboss(float ***filtro); //Cria apenas na ordem 3 (3x3)
void cria_edge_detection(float ***filtro, int ordem);

//Desaloca o filtro criado
void limpa_filtro(float **filtro);

//Funcao analoga a malloc de stdlib.h , porem inicializando um mmap (memoria compartilhada)
void *shalloc(size_t s);

#endif
