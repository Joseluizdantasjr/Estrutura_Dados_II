typedef struct NO *AVL;
#include <stdio.h>
#include <stdlib.h>

struct NO {
  int dado;
  int altura;
  struct NO *esquerdo;
  struct NO *direito;
};
AVL *raiz;
int alturaNO(struct NO *no) {
  if (no == NULL) {
    return -1;
  } else {
    return no->altura;
  }
}
int balanceamento(struct NO *no) {
  int alturaEsquerdo = alturaNO(no->esquerdo);
  int alturaDireito = alturaNO(no->direito);
  int diferenca = alturaEsquerdo - alturaDireito;
  return (diferenca >= 0) ? diferenca : -diferenca;
}
int alturaMaior(struct NO *no1, struct NO *no2) {
  int alturaNo1 = (no1 != NULL) ? alturaNO(no1) : -1;
  int alturaNo2 = (no2 != NULL) ? alturaNO(no2) : -1;

  return (alturaNo1 > alturaNo2) ? alturaNo1 : alturaNo2;
}
void rotacaoDireita(AVL *raiz) {
  struct NO *no;
  no = (*raiz)->esquerdo;
  (*raiz)->esquerdo = no->direito;
  no->direito = *raiz;
  (*raiz)->altura = alturaMaior((*raiz)->esquerdo, (*raiz)->direito) + 1;
  no->altura = alturaMaior(no->esquerdo, (*raiz)) + 1;
  (*raiz) = no;
}
void rotacaoEsquerda(AVL *raiz) {
  struct NO *no;
  no = (*raiz)->direito;
  (*raiz)->direito = no->esquerdo;
  no->esquerdo = *raiz;
  (*raiz)->altura = alturaMaior((*raiz)->esquerdo, (*raiz)->direito) + 1;
  no->altura = alturaMaior(no->direito, (*raiz)) + 1;
  (*raiz) = no;
}

void rotacaoDuplaDireita(AVL *raiz) {
  rotacaoEsquerda(&(*raiz)->esquerdo);
  rotacaoDireita(raiz);
}
void rotacaoDuplaEsquerda(AVL *raiz) {
  rotacaoDireita(&(*raiz)->direito);
  rotacaoEsquerda(raiz);
}
int insereAvl(AVL *raiz, int dado){
  int res;
  if(*raiz == NULL){
    struct NO *new;
    new = (struct NO*)malloc(sizeof(struct NO));
    if(new == NULL){
      return 0;
    }
    new->dado = dado;
    new->altura = 0;
    new->esquerdo = NULL;
    new->direito = NULL;
    *raiz = new;
    return 1;
  }
  struct NO *atual = *raiz;
  if(dado < atual->dado){
    if((res=insereAVL(&(atual->esquerdo), dado))==1){
      if(balanceamento(atual) >= 2){
        if(dado < (*raiz)->esquerdo->dado){
          rotacaoDireita(raiz);
        }
        else{
          rotacaoEsquerda(raiz);
        }
      }
    }
  }
  else{
    if(dado > atual->dado){
      if((res = insereAVL(&(atual->direito), dado)) == 1){
        if(balanceamento(atual) >= 2){
          if((*raiz)->direito->dado < dado){
            rotacaoEsquerda(raiz);
            rotacaoDuplaEsquerda(raiz);
          }
        }
      }
    }
    else{
      printf("Valor Duplicado!!\n");
      return 0;
    }
    
  }
  atual->altura = alturaMaior(atual->esquerdo, atual->direito) + 1;
  return res;
}
