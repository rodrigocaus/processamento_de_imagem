
#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

//Struct imagem, com as matrizes RGB e as dimensoes
typedef struct {
  unsigned int width, height;
  float **r, **g, **b;
} imagem;

//Recebe uma string com o nome do arquivo da imagem e retorna uma struct imagem,
//com as matrizes rgb e as dimensoes
imagem abrir_imagem(char *nome_do_arquivo);

//Transforma a struct imagem recebida uma imagem .jpg com o nome de arquivo especificado
void salvar_imagem(char *nome_do_arquivo, imagem *I);

//Desaloca as matrizes presentes na struct imagem
void liberar_imagem(imagem *i);

#endif
