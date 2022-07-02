# CI752
Trabalho da disciplina CI752 - Fundamentos Lógicos da Inteligência Artificial (feito em 2014, atualizado em 2022)

######## Trabalho da disciplina CI752 ########

Autor: Fabricio Schiavon Kolberg
Professor: Fabiano Silva
Período: 2014/2 (trabalho atualizado para colocação no GitHub em 2022)

Este pequeno documento tem como objetivo dar instruções a respeito da execução dos experimentos,
e também descrever a função de cada arquivo contido nesse diretório.

Todos os programas assumem que os argumentos de entrada estão corretos, já que eles existem 
apenas com o propósito de rodar experimentos.

O arquivo fun_logicos_2.tex é o código fonte do relatório do trabalho, que depende também do 
arquivo fun_logicos.bib.

#=====# Instruções para rodar os experimentos #=====#

1) Compilar todos os códigos fonte, com o comando 'make';
2) Gerar instâncias de teste, com o script './gerar_experimento.sh';
3) Corrigir os endereços dos programas 'nauty' e 'minisat' nos arquivos
   'rodar_experimento_positivo_[kolberg/mugrauer/nauty].sh';
4) Rodar os scripts de experimento 'rodar_experimento_positivo_[kolberg/mugrauer/nauty].sh'.

Os resultados serão encontrados nos arquivos '[kolberg/mugrauer/nauty]_positive.data', descritos
na sequência.



#=====# Descrição dos arquivos #=====#

===== CÓDIGOS FONTE EM C =====

--- codificacao.c ---

Código fonte dos algoritmos de codificação de GI para SAT. 
Uma vez compilado, com o comando
	gcc -o codificacao codificacao.c,
é rodado com a seguinte sintaxe:

./codificacao <GRAFO_1> <GRAFO_2> [m/k]

GRAFO_1 e GRAFO_2 são nomes de arquivos no formato textual descrito no relatório, com o número
de vértices, seguido da matriz de adjacência. Exemplo:

4
0110
1000
1001
0010

"m" e "k" definem se a codificação utilizada é a de Mugrauer e Balint (m), ou a de autoria própria (k).

O programa então retorna, na saída padrão, uma fórmula em DIMACS CNF representando a instância de isomorfismo
entre GRAFO_1 e GRAFO_2.


--- conversao_nauty.c ---

Código fonte do algoritmo que converte dois grafos em um comando nauty para testar isomorfismo. 
Uma vez compilado, com o comando
	gcc -o conversao_nauty conversao_nauty.c,
é rodado com a seguinte sintaxe:

./conversao_nauty <GRAFO_1> <GRAFO_2>

Novamente, GRAFO_1 e GRAFO_2 são nomes de arquivos no formato textual descrito no relatório, com o número
de vértices, seguido da matriz de adjacência.


--- gerar_instancia_positiva.c ---

Código fonte do algoritmo que gera instâncias de isomorfismo baseadas em grafos aleatórios.
Uma vez compilado, com o comando
	gcc -o gerar_instancia_positiva gerar_instancia_positiva.c,
é rodado com a seguinte sintaxe:

./gerar_instancia_positiva n p seed

Onde n é o número desejado de vértices dos grafos da instância positiva, p é a probabilidade de existência de aresta
(que precisa ser um valor com no máximo duas casas decimais, para que a instâncias sejam geradas corretamente), e seed
é um inteiro representando semente de aleatoriedade pedida pelas funções de aleatoriedade da biblioteca padrão do C.

Uma vez rodado, o programa gera dois grafos isomorfos, primeiro criando um grafo aleatório, e depois outro isomorfo
permutando-se seus vértices. Um grafo é devolvido na saída padrão, e outro na saída de erro.



===== SHELL SCRIPTS =====

--- gerar_experimento.sh ---

Script que utiliza-se do programa 'gerar_instancia_positiva' para gerar as instâncias do experimento. Ele utiliza o horário (dado pelo 
comando 'date') para gerar a semente aleatória, e gera uma instância positiva para cada número de vértices entre 10 e 700, e para cada
probabilidade dentro do conjunto {0.1,...,0.9}. Cada instância positiva é composta, então, de dois arquivos: para número de vértices n
e probabilidade p/10, os arquivos grafo_n_p_1 e grafo_n_p_2 representam a instância positiva correspondente.

Para rodá-lo, é necessário que o programa gerar_instancia_positiva tenha sido compilado.


--- rodar_experimento_positivo_[kolberg/mugrauer/nauty].sh ---

Scripts que automatizam a aplicação do experimento e extração de dados do mesmo. Antes de rodá-los, é necessário ter instâncias
geradas pelo script 'gerar_experimento.sh'.

Os scripts 'kolberg' e 'mugrauer' se utilizam do programa 'codificacao' e do software 'minisat' 
para rodar experimentos com a codificação de autoria própria e a de Mugrauer e Balint, respectivamente.
Antes de rodar, mudar o valor da variável END_MINISAT para o endereço do programa minisat.

O script 'nauty' se utiliza do programa 'conversao_nauty' e do 'dreadnaut' pertencente ao pacote nauty para rodar 
experimentos com o algoritmo de McKay. Antes de rodar, mudar o valor da variável END_NAUTY para o endereço do programa
dreadnaut.



===== ARQUIVOS SEM FORMATO =====

--- [kolberg/mugrauer/nauty]_positive.data ---

Contém os resultados dos experimentos para cada instância gerada pelo programa 'gerar_experimento.sh' para cada um dos três
métodos aplicados. A primeira coluna dos arquivos contém o número de vértices da instância correspondente à linha, a segunda
contém a probabilidade (multiplicada por 10) e a terceira contém o tempo real dado pelo comando "time".

