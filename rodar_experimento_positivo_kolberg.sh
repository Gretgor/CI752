#!/bin/bash
#
#Rodar experimento positivo
#

# Substituir o endereço abaixo pelo local onde o sat-solver Minisat está
END_MINISAT="~/Downloads/minisat/core/minisat"

echo -n > kolberg_positive.data
for i in $(seq 10 700)
do
	for j in $(seq 1 9)
	do
	  echo Rodando tamanho $i probabilidade $j
     { time ./codificacao instancias_positivas/grafo_${i}_${j}_1 instancias_positivas/grafo_${i}_${j}_2 k | timeout 5m $END_MINISAT ; } 2> TEMP.OUT
	  TIME=$(grep real TEMP.OUT | tr "\t" " " | cut -d" " -f2)
	  echo $i $j $TIME >> kolberg_positive.data
	done
done
