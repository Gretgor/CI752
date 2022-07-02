#include <stdio.h>
#include <stdlib.h>

/* Conversão do formato de entrada para nauty */

typedef struct{
	int *matriz;
	int num_vertices;
	int num_arestas;
	int *vetor_graus;
	int *vetor_graus_ordenado;
	int *quantidade_nesse_grau;
}grafo;

grafo *le_grafo(FILE *f);

int main(int argc, char **argv){
	/* leitura dos grafos, verificação dos números de vértices e arestas */
	/* argumentos:
     1 - arquivo representando g1
     2 - arquivo representando g2
     */
	
   /* Como esse programa existe apenas para rodar experimentos, eu vou assumir
      que a entrada é sempre certa. */
   int i,j;
	grafo *g1;
	grafo *g2;
	
	FILE *f;
	f=fopen(argv[1],"r");
	g1=le_grafo(f);
	fclose(f);
	f=fopen(argv[2],"r");
	g2=le_grafo(f);
	fclose(f);
	printf("At\n"); /* início da criação do comando nauty */
	printf("c -a V=0\n"); /* opções do nauty */

	printf("n=%d g\n",g1->num_vertices); /* print grafo 1 */
   for(i=0;i<(g1->num_vertices-1);i++){
		for(j=i+1;j<g1->num_vertices;j++){
			if(g1->matriz[i*g1->num_vertices+j]==1)
				printf("%d ",j);
		}
		if(i==(g1->num_vertices-2)){
			printf(".\n");
		}else{
			printf(";\n");
		}
	}
	printf("x @\n"); /* compute, save */

	printf("n=%d g\n",g2->num_vertices); /* print grafo 2 */
   for(i=0;i<(g2->num_vertices-1);i++){
		for(j=i+1;j<g2->num_vertices;j++){
			if(g2->matriz[i*g2->num_vertices+j]==1)
				printf("%d ",j);
		}
		if(i==(g2->num_vertices-2)){
			printf(".\n");
		}else{
			printf(";\n");
		}
	}
	printf("x\n"); /* executa */
	printf("##\n"); /* verifica com o salvo */
	printf("q\n"); /* vaza */
	return 0;
}

grafo *le_grafo(FILE *f){
	int i,j;
	int n;
	char c;
	grafo *g;
	g=malloc(sizeof(grafo));
	fscanf(f,"%d",&n);
	g->num_vertices=n;
	g->num_arestas=0;
	g->matriz=malloc(n*n*sizeof(int));
	i=0;j=0;
	c=fgetc(f);
	while(c != EOF && i<n){
		if(c=='1'){
			g->matriz[i*n+j]=1;
			g->num_arestas++;
			j++;
		}
		if(c=='0'){
			g->matriz[i*n+j]=0;
			j++;
		}
		if(j==n){
			j=0;
			i++;
		}
		c=fgetc(f);
	}
	g->num_arestas=g->num_arestas/2;
	fprintf(stderr,"Grafo tem %d vértices e %d arestas.\n",g->num_vertices,g->num_arestas);
	return g;
}


