#include <stdio.h>
#include <stdlib.h>

typedef struct NO *AVL;

struct NO {
  int dado;
  int altura;
  struct NO *esquerdo;
  struct NO *direito;
};

AVL raiz;

// Calcular altura de um nó
int alturaNO(struct NO *no) {
  if (no == NULL) {
    return -1;
  } else {
    return no->altura;
  }
}

// Calcular fator de balanceamento
int balanceamento(struct NO *no) {
  int alturaEsquerdo = alturaNO(no->esquerdo);
  int alturaDireito = alturaNO(no->direito);
  return alturaEsquerdo - alturaDireito;
}

// Achar qual dos dois nós tem a maior altura
int alturaMaior(struct NO *no1, struct NO *no2) {
  int alturaNo1 = (no1 != NULL) ? alturaNO(no1) : -1;
  int alturaNo2 = (no2 != NULL) ? alturaNO(no2) : -1;
  return (alturaNo1 > alturaNo2) ? alturaNo1 : alturaNo2;
}

// Rotação simples a direita
void rotacaoDireita(AVL *raiz) {
  struct NO *no = (*raiz)->esquerdo;
  (*raiz)->esquerdo = no->direito;
  no->direito = *raiz;
  (*raiz)->altura = alturaMaior((*raiz)->esquerdo, (*raiz)->direito) + 1;
  no->altura = alturaMaior(no->esquerdo, (*raiz)) + 1;
  *raiz = no;
}

// Rotação simples a esquerda
void rotacaoEsquerda(AVL *raiz) {
  struct NO *no = (*raiz)->direito;
  (*raiz)->direito = no->esquerdo;
  no->esquerdo = *raiz;
  (*raiz)->altura = alturaMaior((*raiz)->esquerdo, (*raiz)->direito) + 1;
  no->altura = alturaMaior(no->direito, (*raiz)) + 1;
  *raiz = no;
}

// Rotação dupla a direita
void rotacaoDuplaDireita(AVL *raiz) {
  rotacaoEsquerda(&(*raiz)->esquerdo);
  rotacaoDireita(raiz);
}

// Rotação dupla a esquerda
void rotacaoDuplaEsquerda(AVL *raiz) {
  rotacaoDireita(&(*raiz)->direito);
  rotacaoEsquerda(raiz);
}

// Inserir um nó na árvore
int insereAVL(AVL *raiz, int dado) {
  int res;
  if (*raiz == NULL) {
    struct NO *new = (struct NO *)malloc(sizeof(struct NO));
    if (new == NULL) {
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
  if (dado < atual->dado) {
    if ((res = insereAVL(&(atual->esquerdo), dado)) == 1) {
      if (balanceamento(atual) >= 2) {
        if (dado < (*raiz)->esquerdo->dado) {
          rotacaoDireita(raiz);
        } else {
          rotacaoDuplaDireita(raiz);
        }
      }
    }
  } else if (dado > atual->dado) {
    if ((res = insereAVL(&(atual->direito), dado)) == 1) {
      if (balanceamento(atual) >= 2) {
        if (dado > (*raiz)->direito->dado) {
          rotacaoEsquerda(raiz);
        } else {
          rotacaoDuplaEsquerda(raiz);
        }
      }
    }
  } else {
    printf("Valor Duplicado!!\n");
    return 0;
  }
  atual->altura = alturaMaior(atual->esquerdo, atual->direito) + 1;
  return res;
}

// Acha o menor valor subárvore esquerda
struct NO *procuraMenor(struct NO *atual) {
  while (atual->esquerdo != NULL) {
    atual = atual->esquerdo;
  }
  return atual;
}

// Remove nó
int removeAvl(AVL *raiz, int dado) {
  if (*raiz == NULL) {
    printf("Valor nao existe!!\n");
    return 0;
  }
  int res;
  if (dado < (*raiz)->dado) {
    if ((res = removeAvl(&(*raiz)->esquerdo, dado)) == 1) {
      if (balanceamento(*raiz) >= 2) {
        if (alturaNO((*raiz)->direito->esquerdo) <=
            alturaNO((*raiz)->direito->direito)) {
          rotacaoEsquerda(raiz);
        } else {
          rotacaoDuplaEsquerda(raiz);
        }
      }
    }
  } else if (dado > (*raiz)->dado) {
    if ((res = removeAvl(&(*raiz)->direito, dado)) == 1) {
      if (balanceamento(*raiz) >= 2) {
        if (alturaNO((*raiz)->esquerdo->direito) <=
            alturaNO((*raiz)->esquerdo->esquerdo)) {
          rotacaoDireita(raiz);
        } else {
          rotacaoDuplaDireita(raiz);
        }
      }
    }
  } else {
    if ((*raiz)->esquerdo == NULL || (*raiz)->direito == NULL) {
      struct NO *antigo = *raiz;
      if ((*raiz)->esquerdo != NULL) {
        *raiz = (*raiz)->esquerdo;
      } else {
        *raiz = (*raiz)->direito;
      }
      free(antigo);
    } else {
      struct NO *temp = procuraMenor((*raiz)->direito);
      (*raiz)->dado = temp->dado;
      removeAvl(&(*raiz)->direito, (*raiz)->dado);
      if (balanceamento(*raiz) >= 2) {
        if (alturaNO((*raiz)->esquerdo->direito) <=
            alturaNO((*raiz)->esquerdo->esquerdo)) {
          rotacaoDireita(raiz);
        } else {
          rotacaoDuplaDireita(raiz);
        }
      }
    }
    if (*raiz != NULL) {
      (*raiz)->altura = alturaMaior((*raiz)->esquerdo, (*raiz)->direito) + 1;
    }
    return 1;
  }
  (*raiz)->altura = alturaMaior((*raiz)->esquerdo, (*raiz)->direito) + 1;
  return res;
}

// Função de busca
void buscaNO(AVL raiz, int dado) {
  if (raiz == NULL) {
    printf("Nao encontrado\n");
    return;
  }
  if (dado < raiz->dado) {
    buscaNO(raiz->esquerdo, dado);
  } else if (dado > raiz->dado) {
    buscaNO(raiz->direito, dado);
  } else {
    printf("Encontrado\n");
  }
}

// Imprimir a árvore
void imprimeArvore(AVL raiz) {
  if (raiz != NULL) {
    imprimeArvore(raiz->esquerdo);
    printf("%d ", raiz->dado);
    imprimeArvore(raiz->direito);
  }
}

int main() {
  raiz = NULL;

  int opcao, dado;

  do {
    // Menu de opções
    printf("\nEscolha uma opcao:\n");
    printf("1. Inserir\n");
    printf("2. Remover\n");
    printf("3. Buscar\n");
    printf("4. Imprimir arvore\n");
    printf("5. Sair\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      printf("Digite o valor a ser inserido: ");
      scanf("%d", &dado);
      if (insereAVL(&raiz, dado)) {
        printf("Valor inserido com sucesso!\n");
      } else {
        printf("Falha na insercao!\n");
      }
      break;
    case 2:
      printf("Digite o valor a ser removido: ");
      scanf("%d", &dado);
      if (removeAvl(&raiz, dado)) {
        printf("Valor removido com sucesso!\n");
      } else {
        printf("Falha na remocao!\n");
      }
      break;
    case 3:
      printf("Digite o valor a ser buscado: ");
      scanf("%d", &dado);
      buscaNO(raiz, dado);
      break;
    case 4:
      printf("Arvore em ordem: ");
      imprimeArvore(raiz);
      printf("\n");
      break;
    case 5:
      printf("Saindo...\n");
      break;
    default:
      printf("Opcao invalida! Tente novamente.\n");
      break;
    }
  } while (opcao != 5);

  return 0;
}
