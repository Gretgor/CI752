#!/bin/bash
#
#Código para gerar as instâncias do experimento
#

##### INSTÂNCIAS POSITIVAS #####

mkdir -p instancias_positivas

for i in $(seq 10 700)
do
	for j in $(seq 1 9)
	do
		echo "Gerando instância positiva, n=$i, p=$j/10"
		# gerar instância de tamanho i com probabilidade 0.j
      ./gerar_instancia_positiva $i 0.$j $(date | cut -d":" -f2,3 | cut -d" " -f1 | tr -d ":") > instancias_positivas/grafo_${i}_${j}_1 2> instancias_positivas/grafo_${i}_${j}_2
		if [ "$?" != "0" ]
		then
			echo "Instância positiva não pode ser gerada em $i $j"
			exit
		fi
	done
done

### INSTÂNCIAS NEGATIVAS > basta rodar o gerador de instâncias positivas duas vezes, e rodar os programas sobre grafos gerados em rodagens diferentes.
# Existe uma probabilidade (assintotitcamente minúscula) de uma instância assim gerada ser positiva, mas é pequena o suficiente para ser negligenciada
# para grafos grandes o suficiente.
