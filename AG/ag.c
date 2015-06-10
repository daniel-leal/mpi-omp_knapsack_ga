/*
* @arquivo:           ag.c
* @autores:           Daniel Leal e Rodrigo Andrade
* @instituição:       Centro Universitário do Estado do Pará (CESUPA)
* @descrição:         Análise de Desempenho de um Algortimo Genético para
*                     a solução do problema da mochila booleana.
* @uso:               gcc ag.c -o <output_file> -O -w   //Compila
*                     ./<output_file>                   //Executa
*/

#include "alocacao.c"
#include <time.h>

#define TAM_POP 20                           //tamanho da população (Quantidade de Individuos)
#define TAM_IND 5                            //quantidade de Objetos
#define TAM_TORNEIO 3                         //quantidade de competicoes
#define GERACOES 2                           //numero de Iteracoes
#define PENALIDADE 2                          //penalidade por exceder
#define CAPACIDADE 0.6f                       //capacidade em relacao ao peso total.

int i, j, k;                                  //contador usado nos loops


int main() {

  // Manipulacao de Arquivop
  FILE *arquivo;
  arquivo  = fopen("info.dat","w");

  /*------------------- Declaracao das variaveis------------------------------*/
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




  /* ---------------Construcao do Problema--------------- */
  // Gerar Pesos
  srand((unsigned)time(NULL));
  for (i = 0; i < TAM_IND; i++)
    pesos[i] = rand() % 40;
  // Imprimir Pesos
  fprintf(arquivo, "Pesos: ");
  for(i = 0; i < TAM_IND; i++) {
    fprintf(arquivo, " %d ", pesos[i]);
  }
  fprintf(arquivo, "\n");


  // Gerar Beneficios
  srand((unsigned)time(NULL));
  for (i = 0; i < TAM_IND; i++) {
    beneficios[i] = rand() % 200;
  }
  // Imprimir Beneficios
  fprintf(arquivo, "Valores: ");
  for(i = 0; i < TAM_IND; i++) {
    fprintf(arquivo, " %d ", beneficios[i]);
  }
  fprintf(arquivo, "\n");

  // Calcular a capacidade da mochila
  int peso = 0;
  for (i = 0; i < TAM_IND; i++)
    peso += pesos[i];
  capacidade = peso * CAPACIDADE;
  fprintf(arquivo, "Capacidade da mochila: %d\n", capacidade);
  fprintf(arquivo, "\n");

  /*----------------------- Gerar Populacao --------------------*/
  int auxZero = 0;
  int auxUm = 0;

  for (i = 0; i < TAM_POP; i++) {
    for (j = 0; j < TAM_IND; j++) {
      srand((unsigned)time(NULL));
      pop[i][j] = rand() % 2;
      if (pop[i][j] == 0) // Conta o numero de 0s e 1s do individuo
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
  // Imprimir populacao
  fprintf(arquivo, "Solucao inicial aleatoria:\n");
  for (i = 0 ; i < TAM_POP; i++) {
    fprintf(arquivo, "Individuo%d: ", i);
    for(j = 0; j < TAM_IND; j++) {
      fprintf(arquivo, " %d ", pop[i][j]);
    }
    fprintf(arquivo, "\n");
  }
  fprintf(arquivo, "\n");
  /*----------------------- Fim Gerar Populacao --------------------*/
  /*----------------------Termino Construcao do Problema-------------*/







  /*---------------- Medir qualidade da Solucao Aleatoria-----------------------*/
  // Calcular Peso do Individuo
  int aux = 0;
  for(i = 0; i < TAM_POP; i++) {
    for(j = 0; j < TAM_IND; j++) {
      if (pop[i][j])
        aux += pesos[j];
    }
    pesoIndividuo[i] = aux;
    aux = 0;
  }

  // Calcular Excesso do Individuo
  for(i = 0; i < TAM_POP; i++) {
    if (pesoIndividuo[i] - capacidade <  0)
      excessoIndividuo[i] = 0;
    else
      excessoIndividuo[i] = pesoIndividuo[i] - capacidade;
  }

  // Calcular a Penalidade do Individuo
  aux = 0;
  for(i = 0; i<TAM_POP; i++) {
    for(j = 0 ; j < TAM_IND; j++) {
      if (pop[i][j])
        aux++;
    }
    penalidadeIndividuo[i] = PENALIDADE * aux;
    aux = 0;
  }

  // Calcular Beneficios dos Individuos
  aux = 0;
  for(i = 0; i < TAM_POP; i++) {
    for(j = 0; j < TAM_IND; j++)
      if (pop[i][j])
        aux += beneficios[j] - penalidadeIndividuo[i] * excessoIndividuo[i];
    benefIndividuo[i] = aux;
    aux = 0;
  }

  // Imprime os Dados dos Individuos
  fprintf(arquivo, "Dados de Cada Individuo: \n");
  for(i = 0; i < TAM_POP; i++) {
    fprintf(arquivo, "Individuo%d: ", i);
    fprintf(arquivo, "Peso: %d \t", pesoIndividuo[i]);
    fprintf(arquivo, "Excesso: %d \t", excessoIndividuo[i]);
    fprintf(arquivo, "Fitness: %d ", benefIndividuo[i]);
    fprintf(arquivo, "\n");
  }
  /*---------------- Fim Medir qualidade da Solucao Aleatoria-----------------------*/







  /*----------------------- Torneio (Selecao dos melhores individuos)----------------*/
  int vet[TAM_POP]; //indices populacao intermediaria
  int maior = 0;

  fprintf(arquivo, "\n\nTorneio: \n");
  srand((unsigned)time(NULL));
  for(i = 0; i < TAM_POP; i++) {
    for(j = 0; j < TAM_TORNEIO; j++) {
      torneio[i][j] = rand() % TAM_POP; // Preencher a matriz de torneio com individuos
      fprintf(arquivo, " %d ", torneio[i][j]);
      if (benefIndividuo[torneio[i][j]] > benefIndividuo[maior]) // Analisa qual individuo tem a > fitness
        maior = torneio[i][j];
    }
    fprintf(arquivo, "\n");
    vet[i] = maior;
    maior = 0;
  }

  fprintf(arquivo, "\n");

  //Preencher populacaoIntermediaria
  for(i = 0; i < TAM_POP; i++)
    for(j = 0; j < TAM_IND; j++)
      popIntermed[i][j] = pop[vet[i]][j];

  // Imprimir populacao intermediaria
  fprintf(arquivo, "População Intermediaria - Torneio\n");
  for (i = 0 ; i < TAM_POP; i++) {
    fprintf(arquivo, "Individuo%d: ", i);
    for(j = 0; j < TAM_IND; j++) {
      fprintf(arquivo, " %d ", popIntermed[i][j]);
    }
    fprintf(arquivo, "\n");
  }
  fprintf(arquivo, "\n");
  /*---------------------FIM do Torneio (Selecao dos melhores individuos)-------------*/





  /*------------- Fim Medir qualidade dos Individuos Selecionados ------------------*/
  // Calcular Peso do Individuo
  aux = 0;
  for(i = 0; i < TAM_POP; i++) {
    for(j = 0; j < TAM_IND; j++) {
      if (popIntermed[i][j])
        aux += pesos[j];
    }
    pesoIndividuo[i] = aux;
    aux = 0;
  }

  // Calcular Excesso do Individuo
  for(i = 0; i < TAM_POP; i++) {
    if (pesoIndividuo[i] - capacidade <  0)
      excessoIndividuo[i] = 0;
    else
      excessoIndividuo[i] = pesoIndividuo[i] - capacidade;
  }

  // Calcular a Penalidade do Individuo
  aux = 0;
  for(i = 0; i<TAM_POP; i++) {
    for(j = 0 ; j < TAM_IND; j++) {
      if (popIntermed[i][j])
        aux++;
    }
    penalidadeIndividuo[i] = PENALIDADE * aux;
    aux = 0;
  }

  // Calcular Beneficios dos Individuos
  aux = 0;
  for(i = 0; i < TAM_POP; i++) {
    for(j = 0; j < TAM_IND; j++)
      if (popIntermed[i][j])
        aux += beneficios[j] - penalidadeIndividuo[i] * excessoIndividuo[i];
    benefIndividuo[i] = aux;
    aux = 0;
  }

  // Imprime os Dados dos Individuos
  fprintf(arquivo, "Dados de Cada Individuo: \n");
  for(i = 0; i < TAM_POP; i++) {
    fprintf(arquivo, "Individuo%d: ", i);
    fprintf(arquivo, "Peso: %d \t", pesoIndividuo[i]);
    fprintf(arquivo, "Excesso: %d \t", excessoIndividuo[i]);
    fprintf(arquivo, "Fitness: %d ", benefIndividuo[i]);
    fprintf(arquivo, "\n");
  }
  /*------------- Fim Medir qualidade dos Individuos Selecionados ------------------*/







  /*------------------------------ Cruzamento-------------------------------- */
  fprintf(arquivo, "\n---- Inicio Cruzamento ---\n");
  int indiceMaiorBenef = 0, indiceMenorBenef = 0, indiceSegundoMaiorBenef = 0;
  int maiorBenef = benefIndividuo[0], menorBenef = benefIndividuo[0];
  int segundoMaiorBenef = benefIndividuo[0];
  int mutacao;

  for(k = 0; k < GERACOES; k++) {
    fprintf(arquivo, "\n---- GERACAO: %d -----\n", k);

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

    fprintf(arquivo, "Maior Beneficio: %d\n", maiorBenef);
    fprintf(arquivo, "Indice Maior Benef: %d\n", indiceMaiorBenef);
    fprintf(arquivo, "Segundo Maior Beneficio: %d\n", segundoMaiorBenef);
    fprintf(arquivo, "Indice Segundo Maior Benef: %d\n", indiceSegundoMaiorBenef);
    fprintf(arquivo, "Menor Beneficio: %d\n", menorBenef);
    fprintf(arquivo, "Indice Menor Benef: %d\n", indiceMenorBenef);


    mutacao = rand() % 101;

    for(j = 0; j < TAM_IND / 2; j++)
      if (mutacao > 99) {
        if (popIntermed[indiceMaiorBenef][j] == 0)
          popIntermed[indiceMenorBenef][j] = 1;
        else
          popIntermed[indiceMenorBenef][j] = 0;
      }
      else
        popIntermed[indiceMenorBenef][j] = popIntermed[indiceMaiorBenef][j];

    for(j = TAM_IND / 2; j < TAM_IND; j++)
      if (mutacao > 99) {
        if (popIntermed[indiceSegundoMaiorBenef][j] == 0)
          popIntermed[indiceMenorBenef][j] = 1;
        else
          popIntermed[indiceMenorBenef][j] = 0;
      }
      else
        popIntermed[indiceMenorBenef][j] = popIntermed[indiceSegundoMaiorBenef][j];


    for (i = 0; i < TAM_POP; i++){
      for(j = 0; j < TAM_IND; j++) {
        fprintf(arquivo, " %d ", popIntermed[i][j]);
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

    /*------------------- Medir Qualidade da Geracao-----------------*/
    aux = 0;
    for(i = 0; i < TAM_POP; i++) {
      for(j = 0; j < TAM_IND; j++) {
        if (popIntermed[i][j])
          aux += pesos[j];
      }
      pesoIndividuo[i] = aux;
      aux = 0;
    }

    // Calcular Excesso do Individuo
    for(i = 0; i < TAM_POP; i++) {
      if (pesoIndividuo[i] - capacidade <  0)
        excessoIndividuo[i] = 0;
      else
        excessoIndividuo[i] = pesoIndividuo[i] - capacidade;
    }

    // Calcular a Penalidade do Individuo
    aux = 0;
    for(i = 0; i<TAM_POP; i++) {
      for(j = 0 ; j < TAM_IND; j++) {
        if (popIntermed[i][j])
          aux++;
      }
      penalidadeIndividuo[i] = PENALIDADE * aux;
      aux = 0;
    }

    // Calcular Beneficios dos Individuos
    aux = 0;
    for(i = 0; i < TAM_POP; i++) {
      for(j = 0; j < TAM_IND; j++)
        if (popIntermed[i][j])
          aux += beneficios[j] - penalidadeIndividuo[i] * excessoIndividuo[i];
      benefIndividuo[i] = aux;
      aux = 0;
    }

    // Imprime os Dados dos Individuos
    fprintf(arquivo, "Dados de Cada Individuo: \n");
    for(i = 0; i < TAM_POP; i++) {
      fprintf(arquivo, "Individuo%d: ", i);
      fprintf(arquivo, "Peso: %d \t", pesoIndividuo[i]);
      fprintf(arquivo, "Excesso: %d \t", excessoIndividuo[i]);
      fprintf(arquivo, "Fitness: %d ", benefIndividuo[i]);
      fprintf(arquivo, "\n");
    }
    /*----------------FIM Medir Qualidade da Geracao-----------------*/
  }

  fprintf(arquivo, "\n");


  /*----------Medir Qualidade apos o cruzamento---------------- */

  // Verificar o Melhor Individuo
  indiceMaiorBenef = 0;
  maiorBenef = benefIndividuo[0];
  for(i = 1; i < TAM_POP; i++)
    if (maiorBenef < benefIndividuo[i]) {
      maiorBenef = benefIndividuo[i];
      indiceMaiorBenef = i;
    }
  for (i = 0; i < TAM_IND; i++)
    melhorIndividuo[i] = popIntermed[indiceMaiorBenef][i];


  // Imprimir o Melhor Individuo
  fprintf(arquivo, "\nMelhor Individuo: \n");
  for(i = 0; i < TAM_IND; i++)
    fprintf(arquivo, "%d ", melhorIndividuo[i]);
  /*----------FIM Medir Qualidade apos o cruzamento--------------*/


  fprintf(arquivo, "\n");
  fclose(arquivo);
  /*---------------------------FIM Cruzamento-------------------------------- */

  return 0;
}
