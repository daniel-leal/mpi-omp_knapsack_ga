/*
* @arquivo:           ag.c
* @autores:           Daniel Leal e Rodrigo Andrade
* @instituição:       Centro Universitário do Estado do Pará (CESUPA)
* @descrição:         Análise de Desempenho de um Algortimo Genético para
*                     a solução do problema da mochila booleana.
* @uso:               gcc ag.c -o <output_file> -O -w   //Compila
*                     ./<output_file>                   //Executa
*/
#include <omp.h>
#include "alocacao.c"
#include <time.h>

// ------------------ OpenMP ------------------
#define thread 2
// --------------------------------------------

#define TAM_POP 20                          //tamanho da população (Quantidade de Individuos)
#define TAM_IND 8                           //quantidade de Objetos
#define TAM_TORNEIO 3                       //quantidade de competicoes
#define GERACOES 6                          //numero de Iteracoes
#define PENALIDADE 2                        //penalidade por exceder
#define CAPACIDADE 0.8f                     //capacidade em relacao ao peso total.

int i, j, k;                                //contador usado nos loops
FILE *arquivo;

/* Funcoes */
void popular();
void calcularPesoIndividuo();
void calcularExcessoIndividuo();
void calcularPenalidade();
void calcularBenefIndividuo();
void imprimirDadosIndividuo();
void gerarPesos();
void gerarBeneficio();
void imprimirPop();
void imprimirPesos();
void imprimirBeneficios();
void imprimirMelhorIndividuo();
void verificarMelhorIndividuo();
void competir();
void crossOver();
int calcularCapacidade();
int calcularPesoTotal();

int main() {

  arquivo  = fopen("info.dat","w");

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


  /*Variaveis Sequencial*/
  int CPU;

  /*Feedback para inicialização da CPU*/
  printf("Executando CPU=%d...\n",CPU);

  /*Feedback para inicialização da CPU e Thread*/
  //----------OpenMP---------------------------------------------------------------------------------
  printf("\n-----Existem %d processadores livres da CPU %d!-----\n\n", omp_get_num_procs(), CPU);

  omp_set_num_threads(thread); //definindo a quantidade de threads para a execução do programa
  #pragma omp parallel
  { printf("--------- Thread %d executando na CPU %d! ----------\n", omp_get_thread_num(), CPU); }
  //-------------------------------------------------------------------------------------------------

  /******************************************** PARTE 2 ********************************************/

  if (CPU == 0) {
    gerarPesos(pesos, TAM_IND);
    gerarBeneficio(beneficios, TAM_IND);
    /* Construir o Problema */
    capacidade = calcularCapacidade(pesos, TAM_IND);
    fprintf(arquivo, "\n");
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
    fprintf(arquivo, "População Intermediaria - Torneio\n");
    imprimirPop(popIntermed);
    fprintf(arquivo, "\n");

    /* Medir qualidade dos individuos selecionados */
    calcularPesoIndividuo(popIntermed, pesoIndividuo, pesos);
    calcularExcessoIndividuo(pesoIndividuo, excessoIndividuo, capacidade);
    calcularPenalidade(popIntermed, PENALIDADE, penalidadeIndividuo);
    calcularBenefIndividuo(popIntermed, benefIndividuo, beneficios, penalidadeIndividuo, excessoIndividuo);
    imprimirDadosIndividuo(pesoIndividuo, benefIndividuo, excessoIndividuo);

    /* Cruzamento */
    fprintf(arquivo, "\n---- Inicio Cruzamento ---\n");
  }

  crossOver(popIntermed, benefIndividuo, pesoIndividuo, excessoIndividuo, pesos, capacidade, PENALIDADE, penalidadeIndividuo, beneficios);

  if (CPU == 0) {
    fprintf(arquivo, "\n");
    /* Medir Qualidade apos o cruzamento */
    verificarMelhorIndividuo(popIntermed, benefIndividuo, melhorIndividuo);
    imprimirMelhorIndividuo(melhorIndividuo);
    fprintf(arquivo, "\n");
  }

  fclose(arquivo);

  /*Feedback para finalização da CPU*/
  fprintf(arquivo, "\nConcluido! CPU=%d ok!\n", CPU);

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

  #pragma omp parallel for
  for (i = 0; i < TAM_POP; i++) {
    #pragma omp parallel for
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

  fprintf(arquivo, "Solucao inicial aleatoria:\n");
  imprimirPop(populacao);
  fprintf(arquivo, "\n");
}

/*
* Imprime a população
* @param vet - matriz populacao
*/
void imprimirPop(int vet[][TAM_IND]) {
  for (i = 0 ; i < TAM_POP; i++) {
    fprintf(arquivo, "Individuo%d: ", i);
    for(j = 0; j < TAM_IND; j++) {
      fprintf(arquivo, " %d ", vet[i][j]);
    }
    fprintf(arquivo, "\n");
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
  fprintf(arquivo, "Pesos: ");
  for(i = 0; i < qtde; i++) {
    fprintf(arquivo, " %d ", p[i]);
  }
  fprintf(arquivo, "\n");
}

/*
* Imprime o vetor contendo os valores de cada objeto desejado
* @param b - vetor de beneficios
* @param qtde - quantidade de objetos
*/
void imprimirBeneficios(int b[], int qtde) {
  fprintf(arquivo, "Valores: ");
  for(i = 0; i < qtde; i++) {
    fprintf(arquivo, " %d ", b[i]);
  }
  fprintf(arquivo, "\n");
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
  fprintf(arquivo, "Capacidade da mochila: %d\n", c);

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
  fprintf(arquivo, "Dados de Cada Individuo: \n");
  for(i = 0; i < TAM_POP; i++) {
    fprintf(arquivo, "Individuo%d: ", i);
    fprintf(arquivo, "Peso: %d \t", pesoIndividuo[i]);
    fprintf(arquivo, "Excesso: %d \t", excessoIndividuo[i]);
    fprintf(arquivo, "Fitness: %d ", benefIndividuo[i]);
    fprintf(arquivo, "\n");
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
  fprintf(arquivo, "\nMelhor Individuo: \n");
  for(i = 0; i < TAM_IND; i++)
    fprintf(arquivo, "%d ", melhorIndividuo[i]);
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

  fprintf(arquivo, "\n\nTorneio: \n");
  #pragma omp parallel for
  for(i = 0; i < TAM_POP; i++) {
    #pragma omp parallel for
    for(j = 0; j < TAM_TORNEIO; j++) {
      torneio[i][j] = rand() % TAM_POP; // Preencher a matriz de torneio com individuos
      fprintf(arquivo, " %d ", torneio[i][j]);
      if (benefIndividuo[torneio[i][j]] > benefIndividuo[maior]) // Analisa qual individuo tem a > fitness
        maior = torneio[i][j];
    }
    fprintf(arquivo, "\n");
    aux[i] = maior;
    maior = 0;
  }

  fprintf(arquivo, "\n");

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

  // #pragma omp parallel for
  for(k = 0; k < GERACOES; k++) {
    fprintf(arquivo, "\n---- GERACAO: %d -----\n", k);
    #pragma omp parallel for
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
    #pragma omp parallel for
    for (j = 1; j < TAM_POP; j++) {
      if (benefIndividuo[j] > segundoMaiorBenef && j != indiceMaiorBenef) {
        segundoMaiorBenef = benefIndividuo[j];
        indiceSegundoMaiorBenef = j;
      }
    }

    fprintf(arquivo, "Maior Beneficio: %d\n", maiorBenef);
    fprintf(arquivo, "Indice Maior Benef: %d\n", indiceMaiorBenef);
    fprintf(arquivo, "Segundo Maior Beneficio: %d\n", segundoMaiorBenef);
    fprintf(arquivo, "Indice Segundo Maior Benef: %d\n", indiceSegundoMaiorBenef);
    fprintf(arquivo, "Menor Beneficio: %d\n", menorBenef);
    fprintf(arquivo, "Indice Menor Benef: %d\n", indiceMenorBenef);


    mutacao = rand() % 50;

    if(ehPar(TAM_IND)) {
      #pragma omp parallel for
      for(j = 0; j < TAM_IND / 2; j++)
        if (mutacao > 45) {
          if (populacao[indiceMaiorBenef][j] == 0)
            populacao[indiceMenorBenef][j] = 1;
          else
            populacao[indiceMenorBenef][j] = 0;
        }
        else
          populacao[indiceMenorBenef][j] = populacao[indiceMaiorBenef][j];
      #pragma omp parallel for
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
      #pragma omp parallel for
      for(j = 0; j < (TAM_IND+1) / 2; j++)
        if (mutacao > 45) {
          if (populacao[indiceMaiorBenef][j] == 0)
            populacao[indiceMenorBenef][j] = 1;
          else
            populacao[indiceMenorBenef][j] = 0;
        }
        else
          populacao[indiceMenorBenef][j] = populacao[indiceMaiorBenef][j];
      #pragma omp parallel for
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

    // #pragma omp parallel for
    for (i = 0; i < TAM_POP; i++) {
      // #pragma omp parallel for
      for(j = 0; j < TAM_IND; j++) {
        fprintf(arquivo, " %d ", populacao[i][j]);
      }
      fprintf(arquivo, "\n");
    }

    maiorBenef = benefIndividuo[0];
    segundoMaiorBenef = benefIndividuo[0];
    menorBenef = benefIndividuo[0];
    indiceMaiorBenef = 0;
    indiceMenorBenef = 0;
    indiceSegundoMaiorBenef = 0;

    fprintf(arquivo, "\n");
    calcularPesoIndividuo(populacao, pesoIndividuo, pesos);
    calcularExcessoIndividuo(pesoIndividuo, excessoIndividuo, capacidade);
    calcularPenalidade(populacao, PENALIDADE, penalidadeIndividuo);
    calcularBenefIndividuo(populacao, benefIndividuo, beneficios, penalidadeIndividuo, excessoIndividuo);
    imprimirDadosIndividuo(pesoIndividuo, benefIndividuo, excessoIndividuo);
  }
}
