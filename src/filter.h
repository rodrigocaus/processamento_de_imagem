#ifndef FILTER_H
#define FILTER_H

imagem inicializa_saida(imagem *I);
void aplica_filtro_single(imagem *I, imagem *O, float **filtro, int ordem);
void aplica_filtro_thread(imagem *I, imagem *O, float **filtro, int ordem, int n_thread);

#endif
