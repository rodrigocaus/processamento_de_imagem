
#include <stdlib.h>
#include <stdio.h>

#include "imageprocessing.h"
#include <FreeImage.h>

imagem abrir_imagem(char *nome_do_arquivo) {
  FIBITMAP *bitmapIn;
  int x, y;
  RGBQUAD color;
  imagem I;

  bitmapIn = FreeImage_Load(FIF_JPEG, nome_do_arquivo, 0);

  if (bitmapIn == 0) {
    fprintf(stderr, "Erro! Arquivo não encontrado: %s\n", nome_do_arquivo);
    I.width = I.height = 0;
    I.r = I.g = I.b = NULL;
    return I;
  }

  x = FreeImage_GetWidth(bitmapIn);
  y = FreeImage_GetHeight(bitmapIn);

  I.width = x;  //Numero de colunas
  I.height = y; //Numero de linhas

  //A alocação da matriz é feita dinamicamente
  //Sao apenas dois malloc's:
  //    1 alocação das linhas da matriz
  //    outra alocação para todas as colunas para a primeira linha

  I.r = (float **) malloc(sizeof(float *) * y);
  I.r[0] = (float *) malloc(sizeof(float) * y * x);

  I.g = (float **) malloc(sizeof(float *) * y);
  I.g[0] = (float *) malloc(sizeof(float) * y * x);

  I.b = (float **) malloc(sizeof(float *) * y);
  I.b[0] = (float *) malloc(sizeof(float) * y * x);

  //A cada linha fica atribuida uma seção de colunas
  for(int i = 0; i < y; i++) {
      I.r[i] = (*(I.r) + x*i);
      I.g[i] = (*(I.g) + x*i);
      I.b[i] = (*(I.b) + x*i);
  }

   for (int i=0; i < y; i++) {
     for (int j=0; j < x; j++) {
      FreeImage_GetPixelColor(bitmapIn, j, i, &color);

      I.r[i][j] = color.rgbRed;
      I.g[i][j] = color.rgbGreen;
      I.b[i][j] = color.rgbBlue;
    }
   }
  return I;

}

void liberar_imagem(imagem *I) {
  free(I->r[0]);
  free(I->g[0]);
  free(I->b[0]);
  free(I->r);
  free(I->g);
  free(I->b);
}

void salvar_imagem(char *nome_do_arquivo, imagem *I) {
  FIBITMAP *bitmapOut;
  RGBQUAD color;

  bitmapOut = FreeImage_Allocate(I->width, I->height, 24, 0, 0, 0);

   for (int i=0; i<I->height; i++) {
     for (int j=0; j<I->width; j++) {

      color.rgbRed = I->r[i][j];
      color.rgbGreen = I->g[i][j];
      color.rgbBlue = I->b[i][j];

      FreeImage_SetPixelColor(bitmapOut, j, i, &color);
    }
  }

  FreeImage_Save(FIF_JPEG, bitmapOut, nome_do_arquivo, JPEG_DEFAULT);
}
