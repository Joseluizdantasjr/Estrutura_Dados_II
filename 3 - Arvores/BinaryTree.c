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
struct NoArvore *busca(struct NoArvore *raiz, int valor) {
  if (raiz == NULL || raiz->dado == valor) {
    if (raiz != NULL) {
      printf("Encontrei: %d\n", raiz->dado);
    } else {
      printf("Valor não encontrado.\n");
    }
    return raiz;
  }

  if (valor < raiz->dado) {
    return busca(raiz->esquerda, valor);
  }

  return busca(raiz->direita, valor);
}

// Função para inserir um valor na árvore binária de busca
struct NoArvore *inserir(struct NoArvore *raiz, int valor) {
  if (raiz == NULL) {
    return criarNo(valor);
  }
  if (valor <= raiz->dado) {
    raiz->esquerda = inserir(raiz->esquerda, valor);
  } else {
    raiz->direita = inserir(raiz->direita, valor);
  }
  return raiz;
}

void preOrdem(struct NoArvore *raiz) 
{ 
    while (raiz) 
    { 
        if (raiz->esquerda == NULL) 
        { 
            printf( "%d ", raiz->dado ); 
            raiz = raiz->direita; 
        } 
        else
        { 

            struct NoArvore *temp = raiz->esquerda; 
            while (temp->direita && temp->direita != raiz) 
                temp = temp->direita; 


            if (temp->direita == raiz) 
            { 
                temp->direita = NULL; 
                raiz = raiz->direita; 
            } 

            else
            { 
                printf("%d ", raiz->dado); 
                temp->direita = raiz; 
                raiz = raiz->esquerda; 
            } 
        } 
    } 
} 

// Função para encontrar o nó com o menor valor na árvore
struct NoArvore *encontrarMinimo(struct NoArvore *raiz) {
  struct NoArvore *temp = raiz;
  while (temp && temp->esquerda != NULL) {
    temp = temp->esquerda;
  }
  return temp;
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
      struct NoArvore *temp = raiz->direita;
      free(raiz);
      return temp;
    } else if (raiz->direita == NULL) {
      struct NoArvore *temp = raiz->esquerda;
      free(raiz);
      return temp;
    }

    struct NoArvore *temp = encontrarMinimo(raiz->direita);
    raiz->dado = temp->dado;
    raiz->direita = excluir(raiz->direita, temp->dado);
  }
  return raiz;
}

// Função principal para testar as funções da árvore
int main() {
  struct NoArvore *raiz = NULL;

  // Inserindo elementos na árvore
  for (int i = 1; i <= 10; i++) {
    raiz = inserir(raiz, i);
  }

  printf("Árvore em pré-ordem: ");
  preOrdem(raiz);

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


