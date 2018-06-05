#!/bin/bash

PASTA_ENTRADA=imgs_test
PASTA_SAIDA=imgs_out
EXTENSAO=elf

printf "Entrada\t\t"
printf "Resolução\t"
printf "Procedimento\t"
printf "Numero de T/P\t"
printf "Tempo de operação (ms)\n"

for m in $(ls *.$EXTENSAO);
do
	for img in $(ls $PASTA_ENTRADA/)
	do
		#Remove as extensões dos arquivos
		out=${img%.*}
		processo=${m%.*}
		processo=${processo#main}
		./$m $PASTA_ENTRADA/$img $PASTA_SAIDA/$out$processo.jpg
	done
done
