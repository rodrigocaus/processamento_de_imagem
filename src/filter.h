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
void aplica_filtro_single(imagem *I, imagem *O, float **filtro, int ordem);
void aplica_filtro_threading(imagem *I, imagem *O, float **filtro, int ordem, int n_thread);
void cria_blur(float ***filtro, int ordem);
void cria_emboss(float ***filtro);
void limpa_filtro(float **filtro);
void copia_imagem(imagem *i, imagem *o);

#endif
