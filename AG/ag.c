#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h> //INT_MAX

#define MAX(x,y) ((x)<(y) ? (y) : (x))

int i; //contador usado nos loops

int main()
{
  int n=5;                                  //numero de objetos
  int c = 60;                               //capacidade da mochila
  int s[] = {0,0,0,0,0};                    //solucao corrent
  int s_star[] = {0,0,0,0,0};               //melhor solucao
  int p[] = {52, 23, 35, 15, 7};            //peso de cada objeto
  int b[] = {100, 60, 70, 15, 15};          //benf. de cada objeto
  int fo;                                   //funcao objetivo corrent
  int excesso;                              //excesso de peso na mochila
  int penalidade;                           //penalidade por exceder
  int pesoObjetos;                          //soma dos objetos.

  srand((unsigned) time(NULL));             //pega hora do relogio
  constroi_solucao_aleatoria(s, n);
  printf("Solucao inicial aleatoria:");
  imprime_solucao(s, n);
  printf("\n");

  // Medir qualidade a solucao inicial
  printf("penalidade: %d\n\n", 50);
  printf("beneficio: %d\t peso na mochila: %d\t excesso: %d \n\n", calcula_fo(s, n, b, 50, p, n, c), calcula_peso_objetos(s, n, p), calcula_excesso(calcula_peso_objetos(s, n, p), c));
  printf("\n");

  subida(n,s,b,p,c, penalidade, &pesoObjetos, &fo);
  printf("\nsolucao final:");
  imprime_solucao(s,n);
  printf("\n\n");
  printf("beneficio: %d\t peso na mochila: %d\t excesso: %d \n\n", calcula_fo(s, n, b, 50, p, n, c), calcula_peso_objetos(s, n, p), calcula_excesso(calcula_peso_objetos(s, n, p), c));

  return 0;
}

void constroi_solucao_aleatoria(int s[], int itens) {
  for(i = 0; i<itens; i++){
    s[i] = rand()%2; //escolhe 0 ou 1.
  }
}

void imprime_solucao(int s[], int itens) {
  for(i = 0; i<itens; i++) {
    printf("%d\t", s[i]);
  }
}

int calcula_penalidade(int p[], int itens) {
  int penalidade = 0;
  for(i = 0; i<itens; i++)
    penalidade += p[i];
  return penalidade;
}

int calcula_peso_objetos(int s[], int itens, int p[]) {
  int peso = 0;

  for (i=0; i<itens; i++)
    if(s[i])
      peso += p[i];

  return peso;
}

int calcula_excesso(int peso_objetos, int capacidade) {
  return MAX(0, peso_objetos - capacidade);
}

int calcula_fo(int s[], int itens, int b[], int penalidade, int p[], int n, int c) {

  int peso = calcula_peso_objetos(s, n, p);
  int excesso = calcula_excesso(peso, c);
  int beneficio = 0;

  for(i=0; i<itens; i++) {
    if(s[i])
      beneficio = beneficio + b[i];
  }

  return beneficio - penalidade * excesso;
}

void subida(int n, int s[], int b[], int p[], int c, int penalidade, int *peso_objetos, int *fo) {

  int pos, fo_atual, pesoObjetos, excesso;

  *fo = calcula_fo(s, n, b, penalidade, p, n, c); //vizinho anterior

  for(i=0; i<n; i++) {
    for(pos=0; pos<n; pos++) {
      if(i != pos) {
        troca_bit(s, pos);
        fo_atual = calcula_fo(s, n, b, penalidade, p, n, c); //vizinho atual
        if(fo_atual > *fo) {
          *fo = fo_atual;
          printf("........trocou [%d]\n\n", pos);
        } else {
          troca_bit(s, pos);
          pesoObjetos = calcula_peso_objetos(s,n,p);
          excesso = calcula_excesso(pesoObjetos, c);
        }
      }
    }
  }
}

void troca_bit(int s[], int j) {
  if(s[j])
    s[j] = 0;
  else
    s[j] = 1;
}
