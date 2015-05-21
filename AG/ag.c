/*
* @arquivo:           ag.c
* @autores:           Daniel Leal
* @instituição:       Centro Universitário do Estado do Pará (CESUPA)
* @descrição:         Avaliação de Desempenho de um Algortimo Genético para
*                     a solução do problema da mochila booleana.
* @uso:               gcc Mochila.c -o <output_file> -O -w //Compila
*                     ./<output_file> //Executa
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h> //INT_MAX

#define MAX(x,y) ((x)<(y) ? (y) : (x))

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
                                            //1ª col = indivuo - 2ª col = peso

  /* Fluxo do Programa */
  gerarPesos(pesos, TAM_IND);
  gerarBeneficio(beneficios, TAM_IND);
  capacidade = calcularCapacidade(pesos, TAM_IND);
  printf("\n");
  popular(pop, TAM_POP, TAM_IND);
  calcularPesoIndividuo(pop, pesoIndividuo, TAM_POP, TAM_IND, pesos);
  calcularBenefIndividuo((pop, benefIndividuo, TAM_POP, TAM_IND, beneficios);

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
  int aux = 0, k = 0;
  for(i = 0; i < tamPop; i++) {
    for(j = 0; j < tamIndiv; j++) {
      if (populacao[i][j])
        aux += p[j];
    }
    pesoIndividuo[i] = aux;
    aux = 0;
  }
  imprimirPesoIndividuo(pesoIndividuo, tamPop);
}

void imprimirPesoIndividuo(int pesoIndividuo[], int tamPop) {
  printf("Peso Total de Cada Individuo: \n");
  for(i = 0; i < tamPop; i++) {
    printf("Individuo%d: ", i);
    printf(" %d ", pesoIndividuo[i]);
    printf("\n");
  }
}

/*
* Calcula a soma dos pesos de cada objeto presente na mochila.
* Isto e, todos os elementos com valor 1 de cada individuo.
* @param populacao - populacao de individuos
* @param pesoObjetos - vetor contendo o peso de cada individuo
* @param tamPop - tamanho da populacao
* @param tamIndiv- tamanho do individuo
* @param p - vetor de pesos
*/
void calcularPesoIndividuo(int populacao[][TAM_IND], int pesoIndividuo[], int tamPop, int tamIndiv, int p[]) {
  int aux = 0, k = 0;
  for(i = 0; i < tamPop; i++) {
    for(j = 0; j < tamIndiv; j++) {
      if (populacao[i][j])
        aux += p[j];
    }
    pesoIndividuo[i] = aux;
    aux = 0;
  }
  imprimirPesoIndividuo(pesoIndividuo, tamPop);
}



// int calculaFo(int s[], int itens, int b[], int penalidade, int p[], int n, int c) {
//   int peso = calcula_peso_objetos(s, n, p);
//   int excesso = calcula_excesso(peso, c);
//   int beneficio = 0;
//
//   for(i=0; i<itens; i++) {
//     if(s[i])
//       beneficio = beneficio + b[i];
//   }
//   return beneficio - penalidade * excesso;
// }

// int calcula_penalidade(int p[], int itens) {
//   int penalidade = 0;
//   for(i = 0; i<itens; i++)
//     penalidade += p[i];
//   return penalidade;
// }
//
//

int calcularExcesso(int pesoObjetos, int capacidade) {
  return MAX(0, peso_objetos - capacidade);
}


/*
* Verifica se o número é par.
* @param n - numero a ser testado
* @return 1 (TRUE) caso seja par e 0 (FALSE) caso seja impar.
*/
int ehPar(int n) {
  return n % 2 == 0 ? 1 : 0;
}
