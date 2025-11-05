#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura da sala: árvore binária para mapa da mansão
typedef struct Sala {
    char nome[40];
    char pista[100]; // pista opcional
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Nó da BST para pistas coletadas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

/**
 * Cria uma nova sala com nome e pista (pista pode ser string vazia).
 * Retorna ponteiro para sala alocada dinamicamente.
 */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova != NULL) {
        strncpy(nova->nome, nome, sizeof(nova->nome)-1);
        nova->nome[sizeof(nova->nome)-1] = '\0';
        strncpy(nova->pista, pista, sizeof(nova->pista)-1);
        nova->pista[sizeof(nova->pista)-1] = '\0';
        nova->esquerda = NULL;
        nova->direita = NULL;
    }
    return nova;
}

/**
 * Cria um novo nó para a árvore BST de pistas.
 */
PistaNode* criarPistaNode(const char *pista) {
    PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
    if (novo != NULL) {
        strncpy(novo->pista, pista, sizeof(novo->pista)-1);
        novo->pista[sizeof(novo->pista)-1] = '\0';
        novo->esquerda = NULL;
        novo->direita = NULL;
    }
    return novo;
}

/**
 * Insere uma pista na árvore BST de forma ordenada.
 * Evita pistas duplicadas.
 */
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        return criarPistaNode(pista);
    }
    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (cmp > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // Se cmp == 0 (duplicada), não insere
    return raiz;
}

/**
 * Exibe a árvore BST de pistas em ordem alfabética (in-order).
 */
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

/**
 * Explora o mapa da mansão a partir da sala atual,
 * coletando pistas e armazenando na árvore BST.
 * Navegação interativa com opções esquerda, direita e sair.
 */
void explorarSalasComPistas(Sala *atual, PistaNode **arvorePistas) {
    char opcao;
    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        } else {
            printf("Nenhuma pista nesta sala.\n");
        }
        printf("Escolha para onde ir:\n");
        if (atual->esquerda) printf("  (e) Esquerda: %s\n", atual->esquerda->nome);
        if (atual->direita) printf("  (d) Direita: %s\n", atual->direita->nome);
        printf("  (s) Sair da exploração\n");
        printf("Opção: ");
        scanf(" %c", &opcao);
        if (opcao == 'e' && atual->esquerda) {
            atual = atual->esquerda;
        } else if (opcao == 'd' && atual->direita) {
            atual = atual->direita;
        } else if (opcao == 's') {
            printf("Exploração encerrada, até logo!\n");
            break;
        } else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }
}

/**
 * Libera memória da árvore BST de pistas.
 */
void liberarArvorePistas(PistaNode *raiz) {
    if (raiz != NULL) {
        liberarArvorePistas(raiz->esquerda);
        liberarArvorePistas(raiz->direita);
        free(raiz);
    }
}

/**
 * Libera memória da árvore binária do mapa da mansão.
 */
void liberarMapa(Sala *raiz) {
    if (raiz != NULL) {
        liberarMapa(raiz->esquerda);
        liberarMapa(raiz->direita);
        free(raiz);
    }
}

int main() {
    // Montar mapa fixo da mansão com pistas
    Sala *hall = criarSala("Hall de Entrada", "");
    Sala *salaEstar = criarSala("Sala de Estar", "Pegada misteriosa no tapete");
    Sala *cozinha = criarSala("Cozinha", "Faca suja encontrada");
    Sala *quarto = criarSala("Quarto", "");
    Sala *banheiro = criarSala("Banheiro", "Vidro quebrado no chão");
    Sala *jardim = criarSala("Jardim", "Pegadas levando para a porta dos fundos");
    Sala *biblioteca = criarSala("Biblioteca", "Livro antigo com anotações");

    // Montagem da árvore binária do mapa
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = quarto;
    salaEstar->direita = banheiro;
    cozinha->direita = jardim;
    quarto->direita = biblioteca;

    PistaNode *arvorePistas = NULL;

    printf("\nBem-vindo ao Detective Quest!\nExplore a mansão e colete pistas.\n");

    explorarSalasComPistas(hall, &arvorePistas);

    printf("\nPistas coletadas em ordem alfabética:\n");
    exibirPistas(arvorePistas);

    // Liberar memória alocada
    liberarArvorePistas(arvorePistas);
    liberarMapa(hall);

    return 0;
}
