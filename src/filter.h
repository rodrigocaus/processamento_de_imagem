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

imagem inicializa_saida(imagem *I);
imagem inicializa_saida_shared(imagem *I);
void liberar_imagem_shared(imagem *I);

void aplica_filtro_single(imagem *I, imagem *O, float **filtro, int ordem);
void aplica_filtro_threading(imagem *I, imagem *O, float **filtro, int ordem, int n_thread);
void aplica_filtro_process(imagem *I, imagem *O, float **filtro, int ordem, int n_proc);

void cria_blur(float ***filtro, int ordem);
void cria_emboss(float ***filtro);
void cria_edge_detection(float ***filtro, int ordem);
void limpa_filtro(float **filtro);

void copia_imagem(imagem *i, imagem *o);
void *shalloc(size_t s);

#endif
