#include <stdio.h>
#include <stdlib.h>

/* Codificações de isomorfismo pra SAT */

typedef struct{
	int *matriz;
	int num_vertices;
	int num_arestas;
	int *vetor_graus;
	int *vetor_graus_ordenado;
	int *quantidade_nesse_grau;
}grafo;

void create_cnf_Kolberg(grafo *g1, grafo *g2);
void create_cnf_MugrauerBalint(grafo *g1, grafo *g2);
void calcula_vetor_graus(grafo *g);
void calcula_vetor_graus_sem_ordenar(grafo *g);
void quicksort(grafo *g,int b, int e);
grafo *le_grafo(FILE *f);

int main(int argc, char **argv){
	/* leitura dos grafos, verificação dos números de vértices e arestas */
	/* argumentos:
     1 - arquivo representando g1
     2 - arquivo representando g2
     3 - m: mugrauer-balint, k: kolberg
     4 - 0: construção da lista, 1: textual
     */
	
   /* Como esse programa existe apenas para rodar experimentos, eu vou assumir
      que a entrada é sempre certa. */

	grafo *g1;
	grafo *g2;
	
	FILE *f;
	f=fopen(argv[1],"r");
	g1=le_grafo(f);
	fclose(f);
	f=fopen(argv[2],"r");
	g2=le_grafo(f);

	if(g1->num_vertices != g2->num_vertices || g1->num_arestas != g2->num_arestas){
		fprintf(stderr,"Números de vértices ou arestas não batem, retornando instância insatisfatível padrão.\n");
		printf("p cnf 1 2\n");
		printf("1 0\n");
		printf("-1 0\n");
		return 0;
	}
	fclose(f);
	if(!strcmp(argv[3],"k")){
		create_cnf_Kolberg(g1, g2);
	}else{
		create_cnf_MugrauerBalint(g1, g2);
	}
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

void create_cnf_Kolberg(grafo *g1, grafo *g2){
	calcula_vetor_graus(g1);
	calcula_vetor_graus(g2);
	int i,j,k,l,m,n;
	n=g1->num_vertices;
	m=g1->num_arestas;
	for(i=0;i<g1->num_vertices;i++)
		if(g1->vetor_graus_ordenado[i]!=g2->vetor_graus_ordenado[i]){
			fprintf(stderr,"Vetores de graus não batem, retornando instância insatisfatível padrão.\n");
			printf("p cnf 1 2\n");
			printf("1 0\n");
			printf("-1 0\n");
			return;
		}
	/* se chegar nesse ponto, os vetores de graus são iguais. Bora */
	int num_clausulas=n; /* começa em n para os n mapeamentos at-least */
	int num_vars=0;
	/* contagem de cláusulas e variáveis para fazer o cabeçalho */
	
	num_vars=n*n;
	num_clausulas=n; // o número de cláusulas at-least
	num_clausulas+=((n*(n-1))/2)*n; // o número máximo de cláusulas at-most
	num_clausulas+=m*n;  // para cada aresta, no máximo n cláusulas

	/* se o número de cláusulas é estimado para MAIS, o minisat corrige isso de boa */
	/* se o número de variáveis é estimado para MAIS, idem */
	
	/* todas as informações pegas */
	printf("p cnf %d %d\n",num_vars,num_clausulas);

	/* começar pelas cláusulas at-least*/
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(g1->vetor_graus[i]==g2->vetor_graus[j]){
				printf("%d ", i*n+j+1);
			}
		}	
		printf("0\n");
	}

	/* at-most */
	for(i=0;i<n;i++){
		for(j=i+1;j<n;j++){
			if(g1->vetor_graus[i]==g1->vetor_graus[j]){
				for(k=0;k<n;k++){
					if(g1->vetor_graus[i]==g2->vetor_graus[k]) printf("-%d -%d 0\n",i*n+k+1,j*n+k+1);
				}
			}
		}	
	}

	/* teste vizinhança */
	for(i=0;i<n;i++){
		for(j=i+1;j<n;j++){
			if(g1->matriz[i*n+j]==1){
				for(k=0;k<n;k++){
					if(g1->vetor_graus[i]==g2->vetor_graus[k]){
						/* VAI TER CLÁUSULA */
						for(l=0;l<n;l++){
							if(g1->vetor_graus[j]==g2->vetor_graus[l] && g2->matriz[k*n+l]==1){
								printf("%d ",j*n+l+1);
							}
						}
						printf("-%d 0\n",i*n+k+1);
					}
				}
			}
		}
	}
}

void calcula_vetor_graus(grafo *g){
	int i,j,n;
	n=g->num_vertices;
	g->vetor_graus=malloc(n*sizeof(int));
	g->vetor_graus_ordenado=malloc(n*sizeof(int));
	g->quantidade_nesse_grau=malloc(n*sizeof(int));
	for(i=0;i<n;i++) g->quantidade_nesse_grau[i]=0;
	/* grau de cada cara */
	for(i=0;i<n;i++){
		g->vetor_graus[i]=0;
		g->vetor_graus_ordenado[i]=0;
		for(j=0;j<n;j++){
			g->vetor_graus[i]+=g->matriz[i*n+j];
			g->vetor_graus_ordenado[i]+=g->matriz[i*n+j];
		}
		g->quantidade_nesse_grau[g->vetor_graus[i]]++;
		fprintf(stderr,"O vértice v_%d tem grau %d!\n",i,g->vetor_graus[i]);
	}
	/* ordenação */
	quicksort(g,0,n-1);
}

void calcula_vetor_graus_sem_ordenar(grafo *g){
	int i,j,n;
	n=g->num_vertices;
	g->vetor_graus=malloc(n*sizeof(int));
	g->vetor_graus_ordenado=malloc(n*sizeof(int));
	g->quantidade_nesse_grau=malloc(n*sizeof(int));
	for(i=0;i<n;i++) g->quantidade_nesse_grau[i]=0;
	/* grau de cada cara */
	for(i=0;i<n;i++){
		g->vetor_graus[i]=0;
		g->vetor_graus_ordenado[i]=0;
		for(j=0;j<n;j++){
			g->vetor_graus[i]+=g->matriz[i*n+j];
			g->vetor_graus_ordenado[i]+=g->matriz[i*n+j];
		}
		g->quantidade_nesse_grau[g->vetor_graus[i]]++;
		fprintf(stderr,"O vértice v_%d tem grau %d!\n",i,g->vetor_graus[i]);
	}
}

void quicksort(grafo *g,int b, int e){
	if(b >= e) return; // nothing to do here 
	int pivo,aux,aux2,i,inicio;
	inicio=b;
	pivo=(b+e)/2; /* seleção arbitrária de pivô, não vai ser tão importante assim */
	aux=g->vetor_graus_ordenado[pivo];
	g->vetor_graus_ordenado[pivo]=g->vetor_graus_ordenado[e];
	g->vetor_graus_ordenado[e]=aux;
	for(i=b;i<e;i++){ /* invariante: todos os elementos do vetor de índice menor que i que são menores que o pivô estão,
								no estado corrente do vetor, em índices menores que inicio */
		if(g->vetor_graus_ordenado[i]<aux){
			aux2=g->vetor_graus_ordenado[i];
			g->vetor_graus_ordenado[i]=g->vetor_graus_ordenado[inicio];
			g->vetor_graus_ordenado[inicio]=aux2;
			inicio++;
		}
	}
	aux2=g->vetor_graus_ordenado[inicio];
	g->vetor_graus_ordenado[inicio]=aux;
	g->vetor_graus_ordenado[e]=aux2;
	quicksort(g,b,inicio-1);
	quicksort(g,inicio+1,e);
}

void create_cnf_MugrauerBalint(grafo *g1, grafo *g2){
	int i,j,k,l,m,n;
	int num_vars, num_clausulas;	
	calcula_vetor_graus(g1);
	calcula_vetor_graus(g2);
	n=g1->num_vertices;
	m=g1->num_arestas;
	for(i=0;i<g1->num_vertices;i++)
		if(g1->vetor_graus_ordenado[i]!=g2->vetor_graus_ordenado[i]){
			fprintf(stderr,"Vetores de graus não batem, retornando instância insatisfatível padrão.\n");
			printf("p cnf 1 2\n");
			printf("1 0\n");
			printf("-1 0\n");
			return;
		}

	/* cálculo do número das coisas */
	num_vars=n*n;
	num_clausulas=n; // número de vértices, para cada AT-LEAST
	num_clausulas+=((n*(n-1))/2)*n; // número de AT-MOST (cada par de vértices vezes o número de vértices)
	num_clausulas+=m*(2*(((n*(n-1))/2)-m)); // para cada aresta de G e cada não-aresta de G', vão haver duas cláusulas

	/* se o número de cláusulas é estimado para MAIS, o minisat corrige isso de boa */

	printf("p cnf %d %d\n",num_vars,num_clausulas);
	/* variável i*n+j+1= vértice i mapeado para j */
	
	/* At-least */
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(g1->vetor_graus[i] == g2->vetor_graus[j]){
				printf("%d ",i*n+j+1);
			}
		}
		printf("0\n");
	}

	/* at-most */
	for(i=0;i<n;i++){
		for(j=i+1;j<n;j++){
			if(g1->vetor_graus[i]==g1->vetor_graus[j]){
				for(k=0;k<n;k++){
					if(g1->vetor_graus[i]==g2->vetor_graus[k]) printf("-%d -%d 0\n",i*n+k+1,j*n+k+1);
				}
			}
		}	
	}	

	/* Restrições */
	for(i=0;i<n;i++){
		for(j=i+1;j<n;j++){
			if(g1->matriz[i*n+j]==1){
				for(k=0;k<n;k++){
					if(g1->vetor_graus[j]==g2->vetor_graus[k] || g1->vetor_graus[i]==g2->vetor_graus[k]){
						for(l=k+1;l<n;l++){
							if(g2->matriz[k*n+l]!=1){
								if(g1->vetor_graus[j]==g2->vetor_graus[l] && g1->vetor_graus[i]==g2->vetor_graus[k]){
									printf("-%d -%d 0\n",i*n+k+1,j*n+l+1);
								}
								if(g1->vetor_graus[i]==g2->vetor_graus[l] && g1->vetor_graus[j]==g2->vetor_graus[k]){
									printf("-%d -%d 0\n",i*n+l+1,j*n+k+1);
								}
							}
						}
					}
				}
			}
		}	
	}
}

