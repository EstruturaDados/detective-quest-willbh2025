#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 20

// Estrutura da sala (árvore binária da mansão)
typedef struct Sala {
    char nome[40];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Nó da BST para pistas coletadas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Nó da tabela hash para associar pista a suspeito
typedef struct HashNode {
    char pista[100];
    char suspeito[40];
    struct HashNode *prox;
} HashNode;

HashNode* tabelaHash[TAM_HASH] = { NULL };

/**
 * Função hash simples para strings.
 */
unsigned int funcaoHash(const char *str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % TAM_HASH;
}

/**
 * Insere associação pista - suspeito na tabela hash.
 */
void inserirNaHash(const char *pista, const char *suspeito) {
    unsigned int pos = funcaoHash(pista);
    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    if (novo == NULL) {
        printf("Erro ao alocar memoria para hash.\n");
        return;
    }
    strncpy(novo->pista, pista, sizeof(novo->pista)-1);
    novo->pista[sizeof(novo->pista)-1] = '\0';
    strncpy(novo->suspeito, suspeito, sizeof(novo->suspeito)-1);
    novo->suspeito[sizeof(novo->suspeito)-1] = '\0';
    novo->prox = tabelaHash[pos];
    tabelaHash[pos] = novo;
}

/**
 * Busca suspeito associado a uma pista na tabela hash.
 * Retorna NULL se não encontrar.
 */
const char* encontrarSuspeito(const char *pista) {
    unsigned int pos = funcaoHash(pista);
    HashNode *atual = tabelaHash[pos];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

/**
 * Cria dinamicamente uma sala com nome e pista.
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
    return raiz;
}

/**
 * Exibe a árvore BST de pistas em ordem alfabética.
 */
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

/**
 * Conta quantas pistas da árvore BST apontam para o suspeito dado.
 */
int contarPistasSuspeito(PistaNode *raiz, const char *suspeito) {
    if (raiz == NULL) return 0;
    int count = 0;
    const char *s = encontrarSuspeito(raiz->pista);
    if (s != NULL && strcmp(s, suspeito) == 0)
        count = 1;
    return count + contarPistasSuspeito(raiz->esquerda, suspeito) + contarPistasSuspeito(raiz->direita, suspeito);
}

/**
 * Navega pela mansão, coleta pistas e armazena na BST.
 */
void explorarSalas(Sala *atual, PistaNode **arvorePistas) {
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

/**
 * Libera memória da tabela hash.
 */
void liberarHash() {
    for (int i = 0; i < TAM_HASH; i++) {
        HashNode *atual = tabelaHash[i];
        while (atual != NULL) {
            HashNode *prox = atual->prox;
            free(atual);
            atual = prox;
        }
        tabelaHash[i] = NULL;
    }
}

/**
 * Conduz a fase de julgamento final.
 */
void verificarSuspeitoFinal(PistaNode *arvorePistas) {
    char acusado[40];
    printf("\nDigite o nome do suspeito que deseja acusar: ");
    scanf(" %39[^\n]", acusado);

    int pistasSuspeito = contarPistasSuspeito(arvorePistas, acusado);

    if (pistasSuspeito >= 2) {
        printf("\nVocê tem %d pistas apontando para %s.\n", pistasSuspeito, acusado);
        printf("Parabéns! A acusação está fundamentada. Você encontrou o culpado!\n");
    } else {
        printf("\nVocê tem apenas %d pistas contra %s.\n", pistasSuspeito, acusado);
        printf("Infelizmente, não há pistas suficientes para provar a culpa.\n");
    }
}

int main() {
    // Criação do mapa da mansão com pistas associadas
    Sala *hall = criarSala("Hall de Entrada", "");
    Sala *salaEstar = criarSala("Sala de Estar", "Pegada misteriosa");
    Sala *cozinha = criarSala("Cozinha", "Faca com impressões digitais");
    Sala *quarto = criarSala("Quarto", "Relógio quebrado");
    Sala *banheiro = criarSala("Banheiro", "Vidro quebrado");
    Sala *jardim = criarSala("Jardim", "Pegadas na terra");
    Sala *biblioteca = criarSala("Biblioteca", "Livro rasgado");

    // Montagem da árvore binária do mapa
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = quarto;
    salaEstar->direita = banheiro;
    cozinha->direita = jardim;
    quarto->direita = biblioteca;

    // Associação das pistas aos suspeitos (hash)
    inserirNaHash("Pegada misteriosa", "Suspeito A");
    inserirNaHash("Faca com impressões digitais", "Suspeito B");
    inserirNaHash("Relógio quebrado", "Suspeito A");
    inserirNaHash("Vidro quebrado", "Suspeito C");
    inserirNaHash("Pegadas na terra", "Suspeito B");
    inserirNaHash("Livro rasgado", "Suspeito A");

    PistaNode *arvorePistas = NULL;

    printf("\nBem-vindo ao Detective Quest!\nExplore a mansão, colete pistas e descubra o culpado.\n");

    explorarSalas(hall, &arvorePistas);

    printf("\nPistas coletadas em ordem alfabética:\n");
    exibirPistas(arvorePistas);

    verificarSuspeitoFinal(arvorePistas);

    liberarArvorePistas(arvorePistas);
    liberarMapa(hall);
    liberarHash();

    return 0;
}
