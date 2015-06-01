/*
* @arquivo:           ag.c
* @autores:           Daniel Leal e Rodrigo Andrade
* @instituição:       Centro Universitário do Estado do Pará (CESUPA)
* @descrição:         Análise de Desempenho de um Algortimo Genético para
*                     a solução do problema da mochila booleana.
* @uso:               gcc ag.c -o <output_file> -O -w   //Compila
*                     ./<output_file>                   //Executa
*/

#include "alocacao_complex.c"
#include <time.h>

#define TAM_POP 20                          //tamanho da população (Quantidade de Individuos)
#define TAM_IND 8                           //quantidade de Objetos
#define TAM_TORNEIO 3                       //quantidade de competicoes
#define GERACOES 6                          //numero de Iteracoes
#define PENALIDADE 2                        //penalidade por exceder
#define CAPACIDADE 0.8f                     //capacidade em relacao ao peso total.

int i, j, k;                                //contador usado nos loops

int main() {

  /* Declaracao das variaveis */
  array2d(pop, TAM_POP, TAM_IND);           //populacao de individuos
  array2d(popIntermed, TAM_POP, TAM_IND);   //populacao gerada pelos torneio
  array1d(pesos, TAM_IND);                  //peso de cada objeto
  array1d(beneficios, TAM_IND);             //benf. de cada objeto
  int capacidade;                           //capacidade da mochila
  array1d(pesoIndividuo, TAM_POP);          //soma dos pesos de cada individuo
  array1d(excessoIndividuo, TAM_POP);       //excesso de cada individuo
  array1d(benefIndividuo, TAM_POP);         //beneficio de cada individuo
  array1d(penalidadeIndividuo, TAM_POP);    //penalidade de cada individuo por exceder.
  array1d(melhorIndividuo, TAM_IND);        //melhor individuo
  array2d(torneio, TAM_POP, TAM_TORNEIO);   //matriz das competicoes

  /* Construir o Problema */
  gerarPesos(pesos, TAM_IND);
  gerarBeneficio(beneficios, TAM_IND);
  capacidade = calcularCapacidade(pesos, TAM_IND);
  printf("\n");
  srand((unsigned)time(NULL));
  popular(pop);


  /* Medir qualidade da Solucao Aleatoria */
  calcularPesoIndividuo(pop, pesoIndividuo, pesos);
  calcularExcessoIndividuo(pesoIndividuo, excessoIndividuo, capacidade);
  calcularPenalidade(pop, PENALIDADE, penalidadeIndividuo);
  calcularBenefIndividuo(pop, benefIndividuo, beneficios, penalidadeIndividuo, excessoIndividuo);
  imprimirDadosIndividuo(pesoIndividuo, benefIndividuo, excessoIndividuo);

  /* Torneio (Selecao dos melhores individuos) */
  srand((unsigned)time(NULL));
  competir(pop, popIntermed, benefIndividuo, torneio);
  printf("População Intermediaria - Torneio\n");
  imprimirPop(popIntermed);
  printf("\n");

  /* Medir qualidade dos individuos selecionados */
  calcularPesoIndividuo(popIntermed, pesoIndividuo, pesos);
  calcularExcessoIndividuo(pesoIndividuo, excessoIndividuo, capacidade);
  calcularPenalidade(popIntermed, PENALIDADE, penalidadeIndividuo);
  calcularBenefIndividuo(popIntermed, benefIndividuo, beneficios, penalidadeIndividuo, excessoIndividuo);
  imprimirDadosIndividuo(pesoIndividuo, benefIndividuo, excessoIndividuo);

  /* Cruzamento */
  printf("\n---- Inicio Cruzamento ---\n");
  crossOver(popIntermed, benefIndividuo, pesoIndividuo, excessoIndividuo, pesos, capacidade, PENALIDADE, penalidadeIndividuo, beneficios);
  printf("\n");
  /* Medir Qualidade apos o cruzamento */
  verificarMelhorIndividuo(popIntermed, benefIndividuo, melhorIndividuo);
  imprimirMelhorIndividuo(melhorIndividuo);


  printf("\n");

  return 0;
}

/*
* Inicializa a populacao de individuos
* @param populacao -  matriz contendo os individuos.
*                     As linhas são os individuos e as
*                     colunas sao as caracteristicas
*                     do individuo 1 ou 0 (tem || n tem o objeto na mochila)
*/
void popular(int populacao[][TAM_IND]) {
  // Contador p/ de zeros e ums
  int auxZero = 0;
  int auxUm = 0;

  for (i = 0; i < TAM_POP; i++) {
    for (j = 0; j < TAM_IND; j++) {
      populacao[i][j] = rand() % 2;
      if (populacao[i][j] == 0) // Conta o numero de 0s e 1s do individuo
        auxZero++;
      else
        auxUm++;
    }
    // Caso o individuo seja formado totalmente por 0s ou 1s, renicia-se o laço externo.
    if (auxZero == TAM_IND || auxUm == TAM_IND)
      i--;
    auxZero = 0;
    auxUm = 0;
  }

  printf("Solucao inicial aleatoria:\n");
  imprimirPop(populacao);
  printf("\n");
}

/*
* Imprime a população
* @param vet - matriz populacao
*/
void imprimirPop(int vet[][TAM_IND]) {
  for (i = 0 ; i < TAM_POP; i++) {
    printf("Individuo%d: ", i);
    for(j = 0; j < TAM_IND; j++) {
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
  for (i = 0; i < qtde; i++)
    p[i] = rand() % 50;
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
  int c = peso * CAPACIDADE;
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
* @param p - vetor de pesos
*/
void calcularPesoIndividuo(int populacao[][TAM_IND], int pesoIndividuo[], int p[]) {
  int aux = 0;
  for(i = 0; i < TAM_POP; i++) {
    for(j = 0; j < TAM_IND; j++) {
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
void calcularExcessoIndividuo(int pesoIndividuo[], int excessoIndividuo[], int capacidade) {
  for(i = 0; i < TAM_POP; i++) {
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
* @param benefIndividuo - vetor contendo a soma dos beneficios de cada individuo
* @param b - vetor contendo os beneficios de cada objeto
* @param penalidadeIndividuo - vetor contendo a penalidade por exceder de cada individuo
* @param excessoIndividuo - vetor contendo o valor de excesso de cada individuo
*/
void calcularBenefIndividuo(int populacao[][TAM_IND], int benefIndividuo[], int b[], int penalidadeIndividuo[], int excessoIndividuo[]) {

  int aux = 0;
  for(i = 0; i < TAM_POP; i++) {
    for(j = 0; j < TAM_IND; j++)
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
* @param excessoIndividuo - vetor com o excesso de cada individuo
*/
void imprimirDadosIndividuo(int pesoIndividuo[], int benefIndividuo[], int excessoIndividuo[]) {
  printf("Dados de Cada Individuo: \n");
  for(i = 0; i < TAM_POP; i++) {
    printf("Individuo%d: ", i);
    printf("Peso: %d \t", pesoIndividuo[i]);
    printf("Excesso: %d \t", excessoIndividuo[i]);
    printf("Fitness: %d ", benefIndividuo[i]);
    printf("\n");
  }
}

/*
* Calcula a penalidade por exceder a capacidade da mochila de cada individuo, baseado no
* parametro de penalidade definido pelo usuário
* @param populacao - matriz contendo todos os inviduos
* @param penalidade - penalidade definida pelo usuario
* @param penalidadeIndividuo - vetor a ser preenchido com as penalidades
*/
void calcularPenalidade(int populacao[][TAM_IND], int penalidade, int penalidadeIndividuo[]) {

  int aux = 0;

  for(i = 0; i<TAM_POP; i++) {
    for(j = 0 ; j < TAM_IND; j++) {
      if (populacao[i][j])
        aux++;
    }
    penalidadeIndividuo[i] = penalidade * aux;
    aux = 0;
  }
}

/*
* Verifica o individuo que tem o maior beneficio e guarda num vetor (melhorIndividuo) os itens.
* @param populacao - Conjunto de Individuos
* @param benefIndividuo - vetor com a soma dos beneficios de cada individuo
* @param melhorIndividuo - vetor a ser preenchido com o melhor individuo
*/
void verificarMelhorIndividuo(int populacao[][TAM_IND], int benefIndividuo[], int melhorIndividuo[]) {
  int indiceMaiorBenef = 0;
  int maiorBenef = benefIndividuo[0];

  for(i = 1; i < TAM_POP; i++)
    if (maiorBenef < benefIndividuo[i]) {
      maiorBenef = benefIndividuo[i];
      indiceMaiorBenef = i;
    }

  for (i = 0; i < TAM_IND; i++)
    melhorIndividuo[i] = populacao[indiceMaiorBenef][i];
}

/*
* Imprime o melhor individuo
* @param melhorIndividuo - vetor contendo o melhor indiviudo
*/
void imprimirMelhorIndividuo(int melhorIndividuo[]) {
  printf("\nMelhor Individuo: \n");
  for(i = 0; i < TAM_IND; i++)
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

/*
* Seleciona aleatoriamente N individuos para competirem.
* A fitness define o vencedor dos torneio.
* @param pop - Populacao de inviduos
* @param popIntermed - Populacao gerada pelos torneio
* @param benefIndividuo - vetor com o fitness de cada indiviudo
*/
void competir(int pop[][TAM_IND], int popIntermed[][TAM_IND], int benefIndividuo[], int torneio[][TAM_IND]) {
  int aux[TAM_POP]; //indices populacao intermediaria

  int maior = 0;

  printf("\n\nTorneio: \n");
  for(i = 0; i < TAM_POP; i++) {
    for(j = 0; j < TAM_TORNEIO; j++) {
      torneio[i][j] = rand() % TAM_POP; // Preencher a matriz de torneio com individuos
      printf(" %d ", torneio[i][j]);
      if (benefIndividuo[torneio[i][j]] > benefIndividuo[maior]) // Analisa qual individuo tem a > fitness
        maior = torneio[i][j];
    }
    printf("\n");
    aux[i] = maior;
    maior = 0;
  }

  printf("\n");

  //Preencher populacaoIntermediaria
  for(i = 0; i < TAM_POP; i++)
    for(j = 0; j < TAM_IND; j++)
      popIntermed[i][j] = pop[aux[i]][j];
}

/*
* Combina os individuos da população
*/
void crossOver(int populacao[][TAM_IND], int benefIndividuo[], int pesoIndividuo[], int excessoIndividuo[], int pesos[], int capacidade, int penalidade, int penalidadeIndividuo[], int beneficios[]) {

  int indiceMaiorBenef = 0, indiceMenorBenef = 0, indiceSegundoMaiorBenef = 0;
  int maiorBenef = benefIndividuo[0], menorBenef = benefIndividuo[0], segundoMaiorBenef = benefIndividuo[0];
  int mutacao;

  for(k = 0; k < GERACOES; k++) {
    printf("\n---- GERACAO: %d -----\n", k);

    for(j = 1; j < TAM_POP; j++) {
      if (benefIndividuo[j] > maiorBenef) {
        maiorBenef = benefIndividuo[j];
        indiceMaiorBenef = j;
      }
      if (benefIndividuo[j] < menorBenef) {
        menorBenef = benefIndividuo[j];
        indiceMenorBenef = j;
      }
    }
    for (j = 1; j < TAM_POP; j++) {
      if (benefIndividuo[j] > segundoMaiorBenef && j != indiceMaiorBenef) {
        segundoMaiorBenef = benefIndividuo[j];
        indiceSegundoMaiorBenef = j;
      }
    }

    printf("Maior Beneficio: %d\n", maiorBenef);
    printf("Indice Maior Benef: %d\n", indiceMaiorBenef);
    printf("Segundo Maior Beneficio: %d\n", segundoMaiorBenef);
    printf("Indice Segundo Maior Benef: %d\n", indiceSegundoMaiorBenef);
    printf("Menor Beneficio: %d\n", menorBenef);
    printf("Indice Menor Benef: %d\n", indiceMenorBenef);


    mutacao = rand() % 50;

    if(ehPar(TAM_IND)) {
      for(j = 0; j < TAM_IND / 2; j++)
        if (mutacao > 45) {
          if (populacao[indiceMaiorBenef][j] == 0)
            populacao[indiceMenorBenef][j] = 1;
          else
            populacao[indiceMenorBenef][j] = 0;
        }
        else
          populacao[indiceMenorBenef][j] = populacao[indiceMaiorBenef][j];
      for(j = TAM_IND / 2; j < TAM_IND; j++)
        if (mutacao > 45) {
          if (populacao[indiceSegundoMaiorBenef][j] == 0)
            populacao[indiceMenorBenef][j] = 1;
          else
            populacao[indiceMenorBenef][j] = 0;
        }
        else
          populacao[indiceMenorBenef][j] = populacao[indiceSegundoMaiorBenef][j];
    }
    else {
      for(j = 0; j < (TAM_IND+1) / 2; j++)
        if (mutacao > 45) {
          if (populacao[indiceMaiorBenef][j] == 0)
            populacao[indiceMenorBenef][j] = 1;
          else
            populacao[indiceMenorBenef][j] = 0;
        }
        else
          populacao[indiceMenorBenef][j] = populacao[indiceMaiorBenef][j];
      for(j = (TAM_IND+1)/2; j < TAM_IND; j++)
        if (mutacao > 45) {
          if (populacao[indiceSegundoMaiorBenef][j] == 0)
            populacao[indiceMenorBenef][j] = 1;
          else
            populacao[indiceMenorBenef][j] = 0;
        }
        else
          populacao[indiceMenorBenef][j] = populacao[indiceSegundoMaiorBenef][j];
    }

    for (i = 0; i < TAM_POP; i++){
      for(j = 0; j < TAM_IND; j++) {
        printf(" %d ", populacao[i][j]);
      }
      printf("\n");
    }

    maiorBenef = benefIndividuo[0];
    segundoMaiorBenef = benefIndividuo[0];
    menorBenef = benefIndividuo[0];
    indiceMaiorBenef = 0;
    indiceMenorBenef = 0;
    indiceSegundoMaiorBenef = 0;

    printf("\n");
    calcularPesoIndividuo(populacao, pesoIndividuo, pesos);
    calcularExcessoIndividuo(pesoIndividuo, excessoIndividuo, capacidade);
    calcularPenalidade(populacao, PENALIDADE, penalidadeIndividuo);
    calcularBenefIndividuo(populacao, benefIndividuo, beneficios, penalidadeIndividuo, excessoIndividuo);
    imprimirDadosIndividuo(pesoIndividuo, benefIndividuo, excessoIndividuo);
  }
}
