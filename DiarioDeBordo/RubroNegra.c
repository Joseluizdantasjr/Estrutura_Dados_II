#include <stdio.h>
#include <stdlib.h>

enum Cor { VERMELHO, PRETO };

struct No {
  int dado;
  enum Cor cor;
  struct No *filhos[2];
};

struct No *raiz = NULL;

// Função para criar um novo nó
struct No *criarNo(int dado) {
  struct No *novoNo = (struct No *)malloc(sizeof(struct No));
  novoNo->dado = dado;
  novoNo->cor = VERMELHO;
  novoNo->filhos[0] = novoNo->filhos[1] = NULL;
  return novoNo;
}

// Função para inserir um nó
void inserir(int dado) {
  struct No *pilha[98], *ptr, *novoNo, *xPtr, *yPtr;
  int direcao[98], altura = 0, indice;
  ptr = raiz;
  if (!raiz) {
    raiz = criarNo(dado);
    return;
  }

  pilha[altura] = raiz;
  direcao[altura++] = 0;
  while (ptr != NULL) {
    if (ptr->dado == dado) {
      printf("Duplicatas não permitidas!\n");
      return;
    }
    indice = (dado - ptr->dado) > 0 ? 1 : 0;
    pilha[altura] = ptr;
    ptr = ptr->filhos[indice];
    direcao[altura++] = indice;
  }
  pilha[altura - 1]->filhos[indice] = novoNo = criarNo(dado);
  while (altura >= 3 && pilha[altura - 1]->cor == VERMELHO) {
    if (direcao[altura - 2] == 0) {
      yPtr = pilha[altura - 2]->filhos[1];
      if (yPtr != NULL && yPtr->cor == VERMELHO) {
        pilha[altura - 2]->cor = VERMELHO;
        pilha[altura - 1]->cor = yPtr->cor = PRETO;
        altura -= 2;
      } else {
        if (direcao[altura - 1] == 0) {
          yPtr = pilha[altura - 1];
        } else {
          xPtr = pilha[altura - 1];
          yPtr = xPtr->filhos[1];
          xPtr->filhos[1] = yPtr->filhos[0];
          yPtr->filhos[0] = xPtr;
          pilha[altura - 2]->filhos[0] = yPtr;
        }
        xPtr = pilha[altura - 2];
        xPtr->cor = VERMELHO;
        yPtr->cor = PRETO;
        xPtr->filhos[0] = yPtr->filhos[1];
        yPtr->filhos[1] = xPtr;
        if (xPtr == raiz) {
          raiz = yPtr;
        } else {
          pilha[altura - 3]->filhos[direcao[altura - 3]] = yPtr;
        }
        break;
      }
    } else {
      yPtr = pilha[altura - 2]->filhos[0];
      if (yPtr != NULL && yPtr->cor == VERMELHO) {
        pilha[altura - 2]->cor = VERMELHO;
        pilha[altura - 1]->cor = yPtr->cor = PRETO;
        altura -= 2;
      } else {
        if (direcao[altura - 1] == 1) {
          yPtr = pilha[altura - 1];
        } else {
          xPtr = pilha[altura - 1];
          yPtr = xPtr->filhos[0];
          xPtr->filhos[0] = yPtr->filhos[1];
          yPtr->filhos[1] = xPtr;
          pilha[altura - 2]->filhos[1] = yPtr;
        }
        xPtr = pilha[altura - 2];
        yPtr->cor = PRETO;
        xPtr->cor = VERMELHO;
        xPtr->filhos[1] = yPtr->filhos[0];
        yPtr->filhos[0] = xPtr;
        if (xPtr == raiz) {
          raiz = yPtr;
        } else {
          pilha[altura - 3]->filhos[direcao[altura - 3]] = yPtr;
        }
        break;
      }
    }
  }
  raiz->cor = PRETO;
}

// Função para deletar um nó
void deletar(int dado) {
  struct No *pilha[98], *ptr, *xPtr, *yPtr;
  struct No *pPtr, *qPtr, *rPtr;
  int direcao[98], altura = 0, diff, i;
  enum Cor cor;

  if (!raiz) {
    printf("Árvore não disponível\n");
    return;
  }

  ptr = raiz;
  while (ptr != NULL) {
    if ((dado - ptr->dado) == 0)
      break;
    diff = (dado - ptr->dado) > 0 ? 1 : 0;
    pilha[altura] = ptr;
    direcao[altura++] = diff;
    ptr = ptr->filhos[diff];
  }

  if (!ptr) {
    printf("Valor não encontrado na árvore.\n");
    return; // Valor não encontrado, não faz nada
  }

  if (ptr->filhos[1] == NULL) {
    if ((ptr == raiz) && (ptr->filhos[0] == NULL)) {
      free(ptr);
      raiz = NULL;
    } else if (ptr == raiz) {
      raiz = ptr->filhos[0];
      free(ptr);
    } else {
      pilha[altura - 1]->filhos[direcao[altura - 1]] = ptr->filhos[0];
      free(ptr);
    }
  } else {
    xPtr = ptr->filhos[1];
    if (xPtr->filhos[0] == NULL) {
      xPtr->filhos[0] = ptr->filhos[0];
      cor = xPtr->cor;
      xPtr->cor = ptr->cor;
      ptr->cor = cor;

      if (ptr == raiz) {
        raiz = xPtr;
      } else {
        pilha[altura - 1]->filhos[direcao[altura - 1]] = xPtr;
      }

      direcao[altura] = 1;
      pilha[altura++] = xPtr;
    } else {
      i = altura++;
      while (1) {
        direcao[altura] = 0;
        pilha[altura++] = xPtr;
        yPtr = xPtr->filhos[0];
        if (!yPtr->filhos[0])
          break;
        xPtr = yPtr;
      }

      direcao[i] = 1;
      pilha[i] = yPtr;
      if (i > 0)
        pilha[i - 1]->filhos[direcao[i - 1]] = yPtr;

      yPtr->filhos[0] = ptr->filhos[0];
      xPtr->filhos[0] = yPtr->filhos[1];
      yPtr->filhos[1] = ptr->filhos[1];

      if (ptr == raiz) {
        raiz = yPtr;
      }

      cor = yPtr->cor;
      yPtr->cor = ptr->cor;
      ptr->cor = cor;
    }
  }

  if (altura < 1)
    return;

  if (ptr->cor == PRETO) {
    while (1) {
      pPtr = pilha[altura - 1]->filhos[direcao[altura - 1]];
      if (pPtr && pPtr->cor == VERMELHO) {
        pPtr->cor = PRETO;
        break;
      }

      if (altura < 2)
        break;

      if (direcao[altura - 2] == 0) {
        rPtr = pilha[altura - 1]->filhos[1];
        if (!rPtr)
          break;

        if (rPtr->cor == VERMELHO) {
          pilha[altura - 1]->cor = VERMELHO;
          rPtr->cor = PRETO;
          pilha[altura - 1]->filhos[1] = rPtr->filhos[0];
          rPtr->filhos[0] = pilha[altura - 1];

          if (pilha[altura - 1] == raiz) {
            raiz = rPtr;
          } else {
            pilha[altura - 2]->filhos[direcao[altura - 2]] = rPtr;
          }
          direcao[altura] = 0;
          pilha[altura] = pilha[altura - 1];
          pilha[altura - 1] = rPtr;
          altura++;
          rPtr = pilha[altura - 1]->filhos[1];
        }

        if ((!rPtr->filhos[0] || rPtr->filhos[0]->cor == PRETO) &&
            (!rPtr->filhos[1] || rPtr->filhos[1]->cor == PRETO)) {
          rPtr->cor = VERMELHO;
          altura--;
        } else {
          if (!rPtr->filhos[1] || rPtr->filhos[1]->cor == PRETO) {
            if (rPtr->filhos[0]) {
              rPtr->filhos[0]->cor = PRETO;
            }
            rPtr->cor = VERMELHO;
            pilha[altura - 1]->filhos[1] = rPtr->filhos[0];
            rPtr->filhos[0] = pilha[altura - 1];
          }
          if (pilha[altura - 1] == raiz) {
            raiz = rPtr;
          }
          rPtr->cor = pilha[altura - 1]->cor;
          rPtr->filhos[1]->cor = PRETO;
          pilha[altura - 1]->filhos[1] = rPtr->filhos[0];
          rPtr->filhos[0] = pilha[altura - 1];
          break;
        }
      } else {
        rPtr = pilha[altura - 1]->filhos[0];
        if (!rPtr)
          break;

        if (rPtr->cor == VERMELHO) {
          pilha[altura - 1]->cor = VERMELHO;
          rPtr->cor = PRETO;
          pilha[altura - 1]->filhos[0] = rPtr->filhos[1];
          rPtr->filhos[1] = pilha[altura - 1];

          if (pilha[altura - 1] == raiz) {
            raiz = rPtr;
          } else {
            pilha[altura - 2]->filhos[direcao[altura - 2]] = rPtr;
          }
          direcao[altura] = 1;
          pilha[altura] = pilha[altura - 1];
          pilha[altura - 1] = rPtr;
          altura++;
          rPtr = pilha[altura - 1]->filhos[0];
        }

        if ((!rPtr->filhos[0] || rPtr->filhos[0]->cor == PRETO) &&
            (!rPtr->filhos[1] || rPtr->filhos[1]->cor == PRETO)) {
          rPtr->cor = VERMELHO;
          altura--;
        } else {
          if (!rPtr->filhos[0] || rPtr->filhos[0]->cor == PRETO) {
            if (rPtr->filhos[1]) {
              rPtr->filhos[1]->cor = PRETO;
            }
            rPtr->cor = VERMELHO;
            pilha[altura - 1]->filhos[0] = rPtr->filhos[1];
            rPtr->filhos[1] = pilha[altura - 1];
          }
          if (pilha[altura - 1] == raiz) {
            raiz = rPtr;
          }
          rPtr->cor = pilha[altura - 1]->cor;
          rPtr->filhos[0]->cor = PRETO;
          pilha[altura - 1]->filhos[0] = rPtr->filhos[1];
          rPtr->filhos[1] = pilha[altura - 1];
          break;
        }
      }
    }
  }
}

// Função para imprimir a árvore em ordem
void imprimirEmOrdem(struct No *no) {
  if (no != NULL) {
    imprimirEmOrdem(no->filhos[0]);
    printf("%d (%s) ", no->dado, no->cor == VERMELHO ? "Vermelho" : "Preto");
    imprimirEmOrdem(no->filhos[1]);
  }
}

// Função principal
int main() {
  int opcao, valor;

  while (1) {
    printf("\nMenu:\n");
    printf("1. Inserir\n");
    printf("2. Deletar\n");
    printf("3. Imprimir em ordem\n");
    printf("4. Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      printf("Digite o valor a ser inserido: ");
      scanf("%d", &valor);
      inserir(valor);
      break;
    case 2:
      printf("Digite o valor a ser deletado: ");
      scanf("%d", &valor);
      deletar(valor);
      break;
    case 3:
      printf("Elementos da árvore em ordem: ");
      imprimirEmOrdem(raiz);
      printf("\n");
      break;
    case 4:
      printf("Saindo...\n");
      return 0;
    default:
      printf("Opção inválida!\n");
    }
  }
}
