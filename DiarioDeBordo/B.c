#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define ORDEM 4

struct NoArvoreB {
  int num_chaves;
  int chaves[ORDEM - 1];
  struct NoArvoreB *filhos[ORDEM];
  bool e_folha;
};

// Função para buscar uma chave na árvore B
bool buscarChave(struct NoArvoreB *no, int chave) {
  int i = 0;
  while (i < no->num_chaves && chave > no->chaves[i]) {
    i++;
  }
  if (i < no->num_chaves && chave == no->chaves[i]) {
    return true;
  }
  if (no->e_folha) {
    return false;
  }
  return buscarChave(no->filhos[i], chave);
}

// Função para criar um novo nó
struct NoArvoreB *criarNo(bool e_folha) {
  struct NoArvoreB *novoNo =
      (struct NoArvoreB *)malloc(sizeof(struct NoArvoreB));
  if (novoNo == NULL) {
    perror("Falha na alocação de memória");
    exit(EXIT_FAILURE);
  }
  novoNo->num_chaves = 0;
  novoNo->e_folha = e_folha;
  for (int i = 0; i < ORDEM; i++) {
    novoNo->filhos[i] = NULL;
  }
  return novoNo;
}

// Função para dividir um nó cheio
void dividirFilho(struct NoArvoreB *pai, int indice) {
  struct NoArvoreB *filho = pai->filhos[indice];
  struct NoArvoreB *novoNo = criarNo(filho->e_folha);

  novoNo->num_chaves = ORDEM / 2 - 1;

  for (int i = 0; i < ORDEM / 2 - 1; i++) {
    novoNo->chaves[i] = filho->chaves[i + ORDEM / 2];
  }

  if (!filho->e_folha) {
    for (int i = 0; i < ORDEM / 2; i++) {
      novoNo->filhos[i] = filho->filhos[i + ORDEM / 2];
    }
  }

  filho->num_chaves = ORDEM / 2 - 1;

  for (int i = pai->num_chaves; i > indice; i--) {
    pai->filhos[i + 1] = pai->filhos[i];
  }

  pai->filhos[indice + 1] = novoNo;

  for (int i = pai->num_chaves - 1; i >= indice; i--) {
    pai->chaves[i + 1] = pai->chaves[i];
  }

  pai->chaves[indice] = filho->chaves[ORDEM / 2 - 1];
  pai->num_chaves++;
}

// Função para inserir uma chave em um nó que não está cheio
void inserirNaoCheio(struct NoArvoreB *no, int chave) {
  int i = no->num_chaves - 1;

  if (no->e_folha) {
    while (i >= 0 && no->chaves[i] > chave) {
      no->chaves[i + 1] = no->chaves[i];
      i--;
    }
    no->chaves[i + 1] = chave;
    no->num_chaves++;
  } else {
    while (i >= 0 && no->chaves[i] > chave) {
      i--;
    }
    i++;
    if (no->filhos[i]->num_chaves == ORDEM - 1) {
      dividirFilho(no, i);
      if (no->chaves[i] < chave) {
        i++;
      }
    }
    inserirNaoCheio(no->filhos[i], chave);
  }
}

// Função para inserir uma chave na árvore B
void inserir(struct NoArvoreB **raiz, int chave) {
  struct NoArvoreB *no = *raiz;
  if (no == NULL) {
    *raiz = criarNo(true);
    (*raiz)->chaves[0] = chave;
    (*raiz)->num_chaves = 1;
  } else {
    if (no->num_chaves == ORDEM - 1) {
      struct NoArvoreB *nova_raiz = criarNo(false);
      nova_raiz->filhos[0] = no;
      dividirFilho(nova_raiz, 0);
      *raiz = nova_raiz;
    }
    inserirNaoCheio(*raiz, chave);
  }
}

// Função para imprimir a árvore em um formato estruturado
void imprimirArvore(struct NoArvoreB *no, int nivel) {
  if (no == NULL)
    return;

  for (int i = 0; i < nivel; i++) {
    printf("   ");
  }

  printf("|");
  for (int i = 0; i < no->num_chaves; i++) {
    printf(" %d ", no->chaves[i]);
  }
  printf("|\n");

  if (!no->e_folha) {
    for (int i = 0; i <= no->num_chaves; i++) {
      imprimirArvore(no->filhos[i], nivel + 1);
    }
  }
}

// Função principal para executar o menu
int main() {
  struct NoArvoreB *raiz = NULL;
  int opcao, chave;

  while (1) {
    printf("\nMenu:\n");
    printf("1. Inserir chave\n");
    printf("2. Exibir árvore em formato estruturado\n");
    printf("3. Buscar chave\n");
    printf("4. Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      printf("Digite a chave para inserir: ");
      scanf("%d", &chave);
      inserir(&raiz, chave);
      printf("Chave %d inserida.\n", chave);
      break;
    case 2:
      printf("Árvore B:\n");
      imprimirArvore(raiz, 0);
      printf("\n");
      break;
    case 3:
      printf("Digite a chave para buscar: ");
      scanf("%d", &chave);
      if (buscarChave(raiz, chave)) {
        printf("Chave %d encontrada.\n", chave);
      } else {
        printf("Chave %d não encontrada.\n", chave);
      }
      break;
    case 4:
      printf("Saindo...\n");
      exit(0);
    default:
      printf("Opção inválida. Tente novamente.\n");
    }
  }

  return 0;
}

