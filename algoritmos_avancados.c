#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura da sala: binária, com nome, esquerda e direita
typedef struct Sala {
    char nome[40];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/**
 * Cria uma nova sala com nome especificado.
 * Retorna ponteiro para a sala alocada.
 */
Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova != NULL) {
        strncpy(nova->nome, nome, sizeof(nova->nome)-1);
        nova->nome[sizeof(nova->nome)-1] = '\0'; // garante finalização da string
        nova->esquerda = NULL;
        nova->direita = NULL;
    }
    return nova;
}

/**
 * Permite ao jogador explorar o mapa (árvore binária) interativamente.
 * Em cada etapa mostra o nome da sala e opções de movimento.
 * Sai quando chega a uma sala folha ou o usuário decide sair.
 */
void explorarSalas(Sala *atual) {
    char opcao;
    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Você chegou ao final desse caminho da mansão.\n");
            break;
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
 * Monta o mapa fixo da mansão e inicia a exploração.
 */
int main() {
    // Criação manual do mapa da mansão (estrutura fixa)
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *quarto = criarSala("Quarto");
    Sala *banheiro = criarSala("Banheiro");
    Sala *jardim = criarSala("Jardim");
    Sala *biblioteca = criarSala("Biblioteca");

    // Montagem da estrutura binária:
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = quarto;
    salaEstar->direita = banheiro;
    cozinha->direita = jardim;
    quarto->direita = biblioteca;

    printf("\nBem-vindo ao Detective Quest!\nExplore a mansão e descubra o mistério.\n");

    explorarSalas(hall);

    // Liberação de memória
    free(biblioteca);
    free(quarto);
    free(banheiro);
    free(salaEstar);
    free(jardim);
    free(cozinha);
    free(hall);

    return 0;
}
