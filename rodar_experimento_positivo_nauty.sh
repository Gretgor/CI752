#!/bin/bash
#
#Rodar experimento positivo
#

# Substituir o endereço abaixo pelo local onde o algoritmo de McKay está
END_NAUTY="~/Downloads/nauty25r9/dreadnaut"

echo -n > nauty_positive.data
for i in $(seq 10 700)
do
	for j in $(seq 1 9)
	do
	  echo Rodando tamanho $i probabilidade $j
	  ./conversao_nauty instancias_positivas/grafo_${i}_${j}_1 instancias_positivas/grafo_${i}_${j}_2 > EPICC_TEST
	  # NOTE QUE ESTOU DEIXANDO DE FORA O TEMPO DA CONVERSÃO PARA O FORMATO NAUTY. O motivo para isso é que não
	  # considerei justo ter que fazer o nauty ser testado com o mesmo formato de grafos que eu criei para testar meu
     # algoritmo de codificação. Mesmo assim, o tempo máximo levado por essa conversão, para as maiores instâncias, é
     # sempre bem menos de um segundo.
     { time $END_NAUTY < EPICC_TEST ; } 2> TEMP.OUT
	  TIME=$(grep ^real TEMP.OUT | tr "\t" " " | cut -d" " -f2)
	  echo $i $j $TIME >> nauty_positive.data
	done
done
