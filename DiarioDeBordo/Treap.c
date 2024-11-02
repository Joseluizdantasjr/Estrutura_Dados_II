#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct NoTreap {
  int chave, prioridade;
  struct NoTreap *esq, *dir;
};

// Função para criar um nó com chave e prioridade definidas
struct NoTreap *criarNo(int chave) {
  struct NoTreap *novo = malloc(sizeof(struct NoTreap));
  novo->chave = chave;
  novo->prioridade = rand() % 100;
  novo->esq = novo->dir = NULL;
  return novo;
}

// Função para fazer rotação para baixo para manter a prioridade da Treap
struct NoTreap *rotacionarParaBaixo(struct NoTreap **no, int dir) {
  struct NoTreap *filho = dir ? (*no)->dir : (*no)->esq;
  if (dir) {
    (*no)->dir = filho->esq;
    filho->esq = *no;
  } else {
    (*no)->esq = filho->dir;
    filho->dir = *no;
  }
  *no = filho;
  return filho;
}

// Função para inserir
struct NoTreap *inserirNaTreap(struct NoTreap *raiz, int chave) {
  if (raiz == NULL)
    return criarNo(chave);

  struct NoTreap **atual = &raiz;
  while (*atual != NULL) {
    if (chave < (*atual)->chave) {
      if ((*atual)->esq == NULL) {
        (*atual)->esq = criarNo(chave);
        if ((*atual)->esq->prioridade > (*atual)->prioridade)
          raiz = rotacionarParaBaixo(atual, 0); // Rotação para baixo
        break;
      } else
        atual = &(*atual)->esq;
    } else {
      if ((*atual)->dir == NULL) {
        (*atual)->dir = criarNo(chave);
        if ((*atual)->dir->prioridade > (*atual)->prioridade)
          raiz = rotacionarParaBaixo(atual, 1); // Rotação para baixo
        break;
      } else
        atual = &(*atual)->dir;
    }
  }

  return raiz;
}

// Função de busca
int localizarChave(struct NoTreap *raiz, int chave) {
  return raiz == NULL
             ? 0
             : (raiz->chave == chave ||
                localizarChave(chave < raiz->chave ? raiz->esq : raiz->dir,
                               chave));
}

// Função de remoção
struct NoTreap *removerDaTreap(struct NoTreap *raiz, int chave) {
  if (raiz == NULL)
    return NULL;

  if (chave < raiz->chave) {
    raiz->esq = removerDaTreap(raiz->esq, chave);
  } else if (chave > raiz->chave) {
    raiz->dir = removerDaTreap(raiz->dir, chave);
  } else {
    if (raiz->esq == NULL || raiz->dir == NULL) {
      struct NoTreap *temp = raiz->esq ? raiz->esq : raiz->dir;
      free(raiz);
      return temp;
    }

    struct NoTreap **maiorPrioridade =
        raiz->esq->prioridade > raiz->dir->prioridade ? &raiz->esq : &raiz->dir;
    raiz = rotacionarParaBaixo(maiorPrioridade, maiorPrioridade == &raiz->dir);
    *maiorPrioridade = removerDaTreap(*maiorPrioridade, chave);
  }
  return raiz;
}

// Exibe Treap com indentação em níveis
void exibirTreap(struct NoTreap *raiz, int nivel) {
  if (!raiz)
    return;
  exibirTreap(raiz->dir, nivel + 1);
  for (int i = 0; i < nivel; i++)
    printf("   ");
  printf("%d (P:%d)\n", raiz->chave, raiz->prioridade);
  exibirTreap(raiz->esq, nivel + 1);
}

// Função principal
int main() {
  srand(time(0));
  struct NoTreap *raiz = NULL;
  int opcao, chave;

  while (1) {
    printf("\nMenu:\n");
    printf("1. Inserir chave\n");
    printf("2. Exibir Treap\n");
    printf("3. Buscar chave\n");
    printf("4. Remover chave\n");
    printf("5. Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      printf("Digite a chave para inserir: ");
      scanf("%d", &chave);
      raiz = inserirNaTreap(raiz, chave);
      printf("Chave %d inserida.\n", chave);
      break;
    case 2:
      printf("\nTreap:\n");
      exibirTreap(raiz, 0);
      break;
    case 3:
      printf("Digite a chave para buscar: ");
      scanf("%d", &chave);
      if (localizarChave(raiz, chave)) {
        printf("Chave %d encontrada.\n", chave);
      } else {
        printf("Chave %d não encontrada.\n", chave);
      }
      break;
    case 4:
      printf("Digite a chave para remover: ");
      scanf("%d", &chave);
      raiz = removerDaTreap(raiz, chave);
      printf("Chave %d removida.\n", chave);
      break;
    case 5:
      printf("Saindo...\n");
      exit(0);
    default:
      printf("Opção inválida. Tente novamente.\n");
    }
  }

  return 0;
}
