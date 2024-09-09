#include <stdio.h>
#include <stdlib.h>

// Definição da estrutura do nó da árvore
struct NoArvore {
  int dado;
  struct NoArvore *esquerda;
  struct NoArvore *direita;
};

// Função que cria um novo nó
struct NoArvore *criarNo(int valor) {
  struct NoArvore *novoNo = (struct NoArvore *)malloc(sizeof(struct NoArvore));
  if (novoNo == NULL) {
    printf("Erro ao alocar memória.\n");
    exit(-1);
  }
  novoNo->dado = valor;
  novoNo->esquerda = NULL;
  novoNo->direita = NULL;
  return novoNo;
}

// Função para buscar o nó que representa o valor dado
struct NoArvore *busca(struct NoArvore *atual, int valor) {
  if (atual == NULL || atual->dado == valor) {
    if (atual != NULL) {
      printf("Encontrei: %d\n", atual->dado);
    } else {
      printf("Valor não encontrado.\n");
    }
    return atual;
  }

  if (valor < atual->dado) {
    return busca(atual->esquerda, valor);
  }

  return busca(atual->direita, valor);
}

// Função para inserir um valor na árvore binária de busca
struct NoArvore *inserir(struct NoArvore *atual, int valor) {
  if (atual == NULL) {
    return criarNo(valor);
  }
  if (valor <= atual->dado) {
    atual->esquerda = inserir(atual->esquerda, valor);
  } else {
    atual->direita = inserir(atual->direita, valor);
  }
  return atual;
}

void preOrdem(struct NoArvore *raiz) {
  struct NoArvore *atual = raiz;
  while (atual) {
    if (atual->esquerda == NULL) {
      printf("%d ", atual->dado);
      atual = atual->direita;
    } else {

      struct NoArvore *anterior = atual->esquerda;
      while (anterior->direita && anterior->direita != atual)
        anterior = anterior->direita;

      if (anterior->direita == atual) {
        anterior->direita = NULL;
        atual = atual->direita;
      }

      else {
        printf("%d ", atual->dado);
        anterior->direita = atual;
        atual = atual->esquerda;
      }
    }
  }
  printf("\n");
}

void emOrdem(struct NoArvore *raiz) {
  struct NoArvore *atual = raiz;
  while (atual) {
    if (atual->esquerda == NULL) {
      printf("%d ", atual->dado);
      atual = atual->direita;
    } else {
      struct NoArvore *anterior = atual->esquerda;
      while (anterior->direita && anterior->direita != atual) {
        anterior = anterior->direita;
      }
      if (anterior->direita == NULL) {
        anterior->direita = atual;
        atual = atual->esquerda;
      } else {
        anterior->direita = NULL;
        printf("%d ", atual->dado);
        atual = atual->direita;
      }
    }
  }
  printf("\n");
}

// Função para encontrar o nó com o menor valor na árvore
struct NoArvore *encontrarMinimo(struct NoArvore *raiz) {
  struct NoArvore *anterior = raiz;
  while (anterior && anterior->esquerda != NULL) {
    anterior = anterior->esquerda;
  }
  return anterior;
}

// Função para excluir um nó da árvore binária de busca
struct NoArvore *excluir(struct NoArvore *raiz, int valor) {
  if (raiz == NULL) {
    return raiz;
  }

  if (valor < raiz->dado) {
    raiz->esquerda = excluir(raiz->esquerda, valor);
  } else if (valor > raiz->dado) {
    raiz->direita = excluir(raiz->direita, valor);
  } else {
    if (raiz->esquerda == NULL) {
      struct NoArvore *anterior = raiz->direita;
      free(raiz);
      return anterior;
    } else if (raiz->direita == NULL) {
      struct NoArvore *anterior = raiz->esquerda;
      free(raiz);
      return anterior;
    }

    struct NoArvore *anterior = encontrarMinimo(raiz->direita);
    raiz->dado = anterior->dado;
    raiz->direita = excluir(raiz->direita, anterior->dado);
  }
  return raiz;
}

void imprimirArvore(struct NoArvore *x, int y) {
  if (x == NULL) {
    return;
  }
  imprimirArvore(x->direita, y + 1);
  int i;
  for (i = 0; i < y; i++)
    printf("   ");
  printf("%i\n", x->dado); // Convertendo para caractere para imprimir
  imprimirArvore(x->esquerda, y + 1);
}
// Função principal para testar as funções da árvore
int main() {
  struct NoArvore *raiz = NULL;

  // Inserindo elementos na árvore
  int N = 8, dados[8] = {50, 100, 30, 20, 40, 45, 35, 37};
  for (int i = 0; i < N; i++) {
    raiz = inserir(raiz, dados[i]);
  }
  mostraArvore(raiz, 3);

  printf("Árvore em pré-ordem: ");
  preOrdem(raiz);
  printf("Árvore em ordem: ");
  emOrdem(raiz);

  int menu = 0;
  int valor;

  while (menu != 4) {
    printf("1 - Inserir\n2 - Excluir\n3 - Buscar\n4 - Sair\n");
    scanf("%d", &menu);
    switch (menu) {
    case 1:
      printf("Digite o valor a ser inserido: ");
      scanf("%d", &valor);
      raiz = inserir(raiz, valor);
      break;
    case 2:
      printf("Digite o valor a ser excluído: ");
      scanf("%d", &valor);
      raiz = excluir(raiz, valor);
      break;
    case 3:
      printf("Digite o valor a ser buscado: ");
      scanf("%d", &valor);
      busca(raiz, valor);
      break;
    case 4:
      break;
    }
  }

  return 0;
}
  return 0;
}

