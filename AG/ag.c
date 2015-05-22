/*
* @arquivo:           ag.c
* @autores:           Daniel Leal
* @instituição:       Centro Universitário do Estado do Pará (CESUPA)
* @descrição:         Análise de Desempenho de um Algortimo Genético para
*                     a solução do problema da mochila booleana.
* @uso:               gcc ag.c -o <output_file> -O -w //Compilar
*                     ./<output_file> //Executa
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_POP 7 // Tamanho da população (Quantidade de Individuos)
#define TAM_IND 5 // Quantidade de Objetos
#define GERACOES 10 // Numero de Iteracoes
#define PENALIDADE 50 // penalidade por exceder

int i, j; //contador usado nos loops

int main() {

  /* Declaracao das variaveis */
  int pop[TAM_POP][TAM_IND];                //população de individuos
  int pesos[TAM_IND];                       //peso de cada objeto
  int beneficios[TAM_IND];                  //benf. de cada objeto
  int capacidade;                           //capacidade da mochila
  int pesoIndividuo[TAM_POP];               //soma dos pesos de cada individuo
  int excessoIndividuo[TAM_POP];            //excesso de cada individuo
  int benefIndividuo[TAM_POP];              //beneficio de cada individuo
  int penalidadeIndividuo[TAM_POP];         //penalidade de cada individuo por exceder.
  int melhorIndividuo[TAM_IND];             //melhor individuo

  /* Fluxo do Programa */
  gerarPesos(pesos, TAM_IND);
  gerarBeneficio(beneficios, TAM_IND);
  capacidade = calcularCapacidade(pesos, TAM_IND);
  printf("\n");
  popular(pop, TAM_POP, TAM_IND);
  calcularPesoIndividuo(pop, pesoIndividuo, TAM_POP, TAM_IND, pesos);
  calcularExcessoIndividuo(pesoIndividuo, excessoIndividuo, capacidade, TAM_POP);
  calcularPenalidade(pop, PENALIDADE, TAM_IND, TAM_POP, penalidadeIndividuo);

  /* Medir qualidade da Solucao */
  calcularBenefIndividuo(pop, benefIndividuo, TAM_POP, TAM_IND, beneficios, penalidadeIndividuo, excessoIndividuo);
  imprimirDadosIndividuo(pesoIndividuo, benefIndividuo, excessoIndividuo, TAM_POP);
  verificarMelhorIndividuo(pop, benefIndividuo, TAM_POP, TAM_IND, melhorIndividuo);
  imprimirMelhorIndividuo(melhorIndividuo, TAM_IND);
  printf("\n");

  return 0;
}

/*
* Inicializa a populacao de individuos
* @param populacao -  matriz contendo os individuos.
*                     As linhas são os individuos e as
*                     colunas sao as caracteristicas
*                     do individuo 1 ou 0 (tem || n tem o objeto na mochila)
* @param tamPop - tamanho da populacao
* @param tamIndiv - tamanho do individuo (numero de objetos)
*/
void popular(int populacao[][TAM_IND], int tamPop, int tamIndiv) {
  srand((unsigned)time(NULL));
  for (i = 0; i < tamPop; i++) {
    for (j = 0; j < tamIndiv; j++) {
      populacao[i][j] = rand() % 2; //escolhe 0 ou 1.
    }
  }

  printf("Solucao inicial aleatoria:\n");
  imprimirPop(populacao, tamPop, tamIndiv);
  printf("\n");
}

/*
* Imprime a população
* @param vet - matriz populacao
* @param tamPop - tamanho da populacao
* @param tamIndiv - tamanho do individuo (Qtde de Elementos)
*/
void imprimirPop(int vet[][TAM_IND], int tamPop, int tamIndiv) {
  for (i = 0 ; i < tamPop; i++) {
    printf("Individuo%d: ", i);
    for(j = 0; j < tamIndiv; j++) {
      printf(" %d ", vet[i][j]);
    }
    printf("\n");
  }
}

/*
* Gerar o peso de cada elemento a ser colocado na mochila
* @param p - Vetor que tera os pesos dos objetos
* @param qtd - A quantidade de elementos
*/
void gerarPesos(int p[], int qtde) {
  srand((unsigned)time(NULL));
  for (i = 0; i < qtde; i++) {
    p[i] = rand() % 50;
  }
  imprimirPesos(p, qtde);
}

/*
* Gerar o peso de cada elemento a ser colocado na mochila
* @param b - Vetor que tera os beneficios dos objetos
* @param qtd - A quantidade de elementos
*/
void gerarBeneficio(int b[], int qtde) {
  srand((unsigned)time(NULL));
  for (i = 0; i < qtde; i++) {
    b[i] = rand() % 100;
  }
  imprimirBeneficios(b, qtde);
}

/*
* Imprime o vetor contendo os pesos de cada objeto desejado
* @param p - vetor de pesos
* @param qtde - quantidade de objetos
*/
void imprimirPesos(int p[], int qtde) {
  printf("Pesos: ");
  for(i = 0; i < qtde; i++) {
    printf(" %d ", p[i]);
  }
  printf("\n");
}

/*
* Imprime o vetor contendo os valores de cada objeto desejado
* @param b - vetor de beneficios
* @param qtde - quantidade de objetos
*/
void imprimirBeneficios(int b[], int qtde) {
  printf("Valores: ");
  for(i = 0; i < qtde; i++) {
    printf(" %d ", b[i]);
  }
  printf("\n");
}

/*
* Calcula a capacidade da mochila, baseando-se nos pesos dos objetos.
* A capacidade é equivalente a 60% da soma dos pesos.
* @param p - Vetor de pesos
* @param qtde - Quantidade de objetos
* @return c - capacidade da mochila
*/
int calcularCapacidade(int p[], int qtde) {
  int peso = calcularPesoTotal();
  int c = peso * 0.6;
  printf("Capacidade da mochila: %d\n", c);

  return c;
}

/*
* Calcula o peso de todos os objetos desejados
* @param p - vetor de pesos
* @param qtde - quantidade de objetos
* @return peso - soma dos pesos
*/
int calcularPesoTotal(int p[], int qtde) {
  int peso = 0;

  for (i = 0; i < qtde; i++)
    peso += p[i];

  return peso;
}

/*
* Calcula a soma dos pesos de cada objeto presente na mochila.
* Isto é, todos os elementos com valor 1 de cada individuo.
* @param populacao - populacao de individuos
* @param pesoObjetos - vetor contendo o peso de cada individuo
* @param tamPop - tamanho da populacao
* @param tamIndiv- tamanho do individuo
* @param p - vetor de pesos
*/
void calcularPesoIndividuo(int populacao[][TAM_IND], int pesoIndividuo[], int tamPop, int tamIndiv, int p[]) {
  int aux = 0;
  for(i = 0; i < tamPop; i++) {
    for(j = 0; j < tamIndiv; j++) {
      if (populacao[i][j])
        aux += p[j];
    }
    pesoIndividuo[i] = aux;
    aux = 0;
  }
}

/*
* Calcula o execesso de peso de cada individuo.
* 0 caso não haja excesso.
* @pram pesoIndividuo - vetor com o peso dos individuos
* @param excessoIndividuo - vetor que sera preenchido com o excesso de peso dos individuos
* @param capacidade - capacidade de peso da mochila.
*/
void calcularExcessoIndividuo(int pesoIndividuo[], int excessoIndividuo[], int capacidade, int tamPop) {
  for(i = 0; i < tamPop; i++) {
    if (pesoIndividuo[i] - capacidade <  0)
      excessoIndividuo[i] = 0;
    else
      excessoIndividuo[i] = pesoIndividuo[i] - capacidade;
  }
}

/*
* Calcula a soma dos pesos de cada objeto presente na mochila.
* Isto é, todos os elementos com valor 1 de cada individuo.
* @param populacao - populacao de individuos
* @param pesoObjetos - vetor contendo o peso de cada individuo
* @param tamPop - tamanho da populacao
* @param tamIndiv- tamanho do individuo
* @param p - vetor de pesos
*/
void calcularBenefIndividuo(int populacao[][TAM_IND], int benefIndividuo[], int tamPop, int tamIndiv, int b[], int penalidadeIndividuo[], int excessoIndividuo[]) {

  int aux = 0;
  for(i = 0; i < tamPop; i++) {
    for(j = 0; j < tamIndiv; j++)
      if (populacao[i][j])
        aux += b[j] - penalidadeIndividuo[i] * excessoIndividuo[i];
    benefIndividuo[i] = aux;
    aux = 0;
  }
}

/*
* Imprime o peso e o beneficio de cada individuo
* @param pesoIndividuo - vetor com o peso de cada individuo
* @param benefIndividuo - vetor com o beneficio de cada individuo
* @param tamPop - Quantidade de objetos (Individuos)
*/
void imprimirDadosIndividuo(int pesoIndividuo[], int benefIndividuo[], int excessoIndividuo[], int tamPop) {
  printf("Dados de Cada Individuo: \n");
  for(i = 0; i < tamPop; i++) {
    printf("Individuo%d: ", i);
    printf("Peso: %d \t", pesoIndividuo[i]);
    printf("Excesso: %d \t", excessoIndividuo[i]);
    printf("Benef: %d ", benefIndividuo[i]);
    printf("\n");
  }
}


void calcularPenalidade(int populacao[][TAM_IND], int penalidade, int tamIndiv, int tamPop, int penalidadeIndividuo[]) {

  int aux = 0;

  for(i = 0; i<tamPop; i++) {
    for(j = 0 ; j < tamIndiv; j++) {
      if (populacao[i][j])
        aux++;
    }
    penalidadeIndividuo[i] = penalidade * aux;
    aux = 0;
  }
}


void verificarMelhorIndividuo(int populacao[][TAM_IND], int benefIndividuo[], int tamPop, int tamIndiv, int melhorIndividuo[]) {
  int indiceMaiorBenef = 0;
  int maiorBenef = benefIndividuo[0];

  for(i = 1; i < tamPop; i++)
    if (maiorBenef < benefIndividuo[i]) {
      maiorBenef = benefIndividuo[i];
      indiceMaiorBenef = i;
    }

  for (i = 0; i < tamIndiv; i++)
    melhorIndividuo[i] = populacao[indiceMaiorBenef][i];
}


void imprimirMelhorIndividuo(int melhorIndividuo[], int tamIndiv) {
  printf("\nMelhor Individuo: \n");
  for(i = 0; i < tamIndiv; i++)
    printf("%d ", melhorIndividuo[i]);
}


/*
* Verifica se o número é par.
* @param n - numero a ser testado
* @return 1 (TRUE) caso seja par e 0 (FALSE) caso seja impar.
*/
int ehPar(int n) {
  return n % 2 == 0 ? 1 : 0;
}
