#include <stdio.h>
#include <stdlib.h>

/* programa para gerar instâncias positivas de isomorfismo a partir de grafos aleatórios, yay */

/* entradas: ordem, probabilidade de aresta, semente aleatória */

void permutar(int *v, int n);
double randfrom(double min, double max);

int main(int argc, char **argv){
	int n,i,j;
	int *mat1;
	int *mat2;
	int *permutacao;
	double p;
	int seed;

   n=atoi(argv[1]);
	p=atof(argv[2]);
	seed=atoi(argv[3]);
	srand(seed);
	mat1=malloc(n*n*sizeof(int));
	mat2=malloc(n*n*sizeof(int));
	permutacao=malloc(n*sizeof(int));
	permutar(permutacao,n);
	
	/* finalmente gerar os grafos isomorfos */
	
	for(i=0;i<n;i++){
		mat1[i*n+i]=0;
		mat2[i*n+i]=0;
		for(j=i+1;j<n;j++){
			if(rand()%100<=p*100){ // não consegui gerar randoms double direito 
				mat1[i*n+j]=1;
				mat2[permutacao[i]*n+permutacao[j]]=1;
				mat1[j*n+i]=1;
				mat2[permutacao[j]*n+permutacao[i]]=1;
			}else{
				mat1[i*n+j]=0;
				mat2[permutacao[i]*n+permutacao[j]]=0;
				mat1[j*n+i]=0;
				mat2[permutacao[j]*n+permutacao[i]]=0;
			}
		}
	}

	printf("%d\n",n);
	fprintf(stderr,"%d\n",n); /* grafo 1 sai na saída padrão, grafo 2 sai na saída de erro, gambiarra!!! */
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			printf("%d",mat1[i*n+j]);
			fprintf(stderr,"%d",mat2[i*n+j]);
		}
		printf("\n");
		fprintf(stderr,"\n");
	}
	return 0;
}


/* gera uma permutação do vetor [0..n-1] */
void permutar(int *v, int n){
	int i;
	for(i=0;i<n;i++) v[i]=i;
	int j,swap;
	int n_t; /* todas as permutações possíveis podem ser obtidas através de no máximo n trocas */
	n_t=n;
	while(n_t>0){
		i=rand()%n;
		j=rand()%n;
		swap=v[i];
		v[i]=v[j];
		v[j]=swap;
		n_t--;
	}
	return;
}
