#ifndef FILTER_H
#define FILTER_H

imagem inicializa_saida(imagem *I);
void aplica_filtro(imagem *I, imagem *O, float **filtro, int ordem);

#endif
