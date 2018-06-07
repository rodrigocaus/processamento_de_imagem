# Processamento de Imagem

O objetivo do trabalho é implementar um programa capaz de
aplicar um filtro em uma imagem por um processo de
convolução. Serão feitos três procedimentos: um processamento
simples feito em um processo único, um processamento feito
com multiprocessos, utilizando `fork`, e um processamento
feito com multithreads, utilizando a biblioteca `pthread`

## Teste do programa

O programa é executado com dois argumentos: um arquivo de
entrada e outro de saída, da forma:

```
./main imagem_entrada.jpg imagem_saida_procedimento.jpg
```

Com o Makefile, serão testados todos as imagens em todos os
procedimentos a partir do comando

```
make test
```

A saída é o nome do arquivo de entrada, sua resolução, o
procedimento empregado, número de threads/processos, ordem da
matriz de convolução e o tempo de operação sobre a imagem de entrada.

## Processamento de imagem

Os procedimentos de abrir e salvar as imagens estão nos aquivos `imageprocessing`
implementados pelo professor Tiago Tavares, com uso da biblioteca `FreeImage.h`.
Os pacotes libfreeimage-dev e libfreeimage3 devem ser instalados para o funcionamento
dos programas.
