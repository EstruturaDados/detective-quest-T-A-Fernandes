#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Para a função tolower()

// ------------------------------------------
// 1. CONSTANTES
// ------------------------------------------
#define MAX_NOME 50
#define MAX_PISTA 100

// ------------------------------------------
// 2. FUNÇÕES DE CRIAÇÃO DO MAPA
// ------------------------------------------

/**
 * @brief Cria e inicializa um novo cômodo (nó) da mansão.
 *
 * Requisito: A sala é criada de forma dinâmica e possui um nome.
 * @param nome O nome do cômodo a ser criado.
 * @return Sala* Um ponteiro para a nova sala criada.
 */
Sala* criarSala(const char *nome) {
    // Aloca dinamicamente o espaço para a nova sala
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));

    // Verifica se a alocação foi bem-sucedida
    if (novaSala == NULL) {
        perror("Erro ao alocar memória para a sala.");
        exit(EXIT_FAILURE); // Encerra o programa em caso de erro fatal
    }

    // Copia o nome para o campo 'nome' da struct
    strncpy(novaSala->nome, nome, 49);
    novaSala->nome[49] = '\0'; // Garante terminação da string

    // Inicializa os ponteiros dos filhos como NULL (sem caminhos inicialmente)
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

/**
 * @brief Constrói o mapa fixo da mansão como uma Árvore Binária.
 *
 * Requisito: A estrutura da mansão já vem definida no código.
 * @return Sala* O ponteiro para o nó Raiz da mansão (Hall de Entrada).
 */
Sala* montarMapa() {
    // Nível 0: Raiz (Hall de Entrada)
    Sala *hall = criarSala("Hall de Entrada");

    // Nível 1
    hall->esquerda = criarSala("Sala de Estar");
    hall->direita = criarSala("Cozinha");

    // Nível 2
    hall->esquerda->esquerda = criarSala("Quarto Principal"); // Esquerda da Sala de Estar
    hall->esquerda->direita = criarSala("Biblioteca");        // Direita da Sala de Estar

    hall->direita->direita = criarSala("Dispensa");           // Direita da Cozinha (não tem filho esquerdo)

    // Nível 3
    hall->esquerda->esquerda->esquerda = criarSala("Banheiro"); // Esquerda do Quarto Principal
    // O Quarto Principal->direita (Direita) fica como NULL (nó folha)

    hall->esquerda->direita->direita = criarSala("Jardim");   // Direita da Biblioteca (não tem filho esquerdo)

    // Nível 4 (Folhas: Banheiro e Jardim já são folhas)
    // Criando uma folha a partir do Banheiro (para demonstrar mais profundidade)
    hall->esquerda->esquerda->esquerda->esquerda = criarSala("Subsolo (Folha)");
    
    return hall;
}

// ------------------------------------------
// 3. FUNÇÃO DE EXPLORAÇÃO INTERATIVA
// ------------------------------------------

/**
 * @brief Permite a navegação interativa do jogador pela mansão (Árvore).
 *
 * Requisito: A exploração continua até o jogador alcançar um nó-folha.
 * @param raiz O nó raiz da árvore (Hall de Entrada).
 */
void explorarSalas(Sala *raiz) {
    Sala *atual = raiz;
    char opcao;

    printf("\n>>> BEM-VINDO(A) ao Detective Quest! <<<\n");
    printf("Você deve explorar a mansão para encontrar o culpado.\n\n");

    // Loop de exploração: continua enquanto o jogador estiver em uma sala válida
    while (atual != NULL) {
        printf("===========================================\n");
        printf("VOCÊ ESTA EM: %s\n", atual->nome);

        // Verifica se é um nó-folha (sem caminhos)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("\n[FIM DA EXPLORAÇÃO] Você chegou a um comodo sem mais saidas (No Folha).\n");
            printf("A investigação neste caminho esta completa!\n");
            break; // Encerra o loop, encerrando o jogo
        }

        printf("-------------------------------------------\n");
        printf("Opções de caminho:\n");

        // Informa as opções baseadas nos caminhos disponíveis
        if (atual->esquerda != NULL) {
            printf(" [E] Esquerda: %s\n", atual->esquerda->nome);
        }
        if (atual->direita != NULL) {
            printf(" [D] Direita: %s\n", atual->direita->nome);
        }
        printf(" [S] Sair do Jogo.\n");
        printf("-------------------------------------------\n");
        printf("Sua escolha (e/d/s): ");

        // Lendo a opção do usuário
        if (scanf(" %c", &opcao) != 1) {
            // Limpa o buffer em caso de erro de leitura
            while (getchar() != '\n');
            continue; // Volta ao início do loop
        }
        
        // Converte a opção para minúscula para simplificar a comparação
        opcao = tolower(opcao);

        // Controle das decisões do jogador
        if (opcao == 'e') {
            if (atual->esquerda != NULL) {
                // Navega para o cômodo da esquerda
                atual = atual->esquerda;
            } else {
                printf("[ALERTA] Não há caminho a esquerda a partir daqui! Tente outra opcao.\n");
            }
        } else if (opcao == 'd') {
            if (atual->direita != NULL) {
                // Navega para o cômodo da direita
                atual = atual->direita;
            } else {
                printf("[ALERTA] Não há caminho a direita a partir daqui! Tente outra opção.\n");
            }
        } else if (opcao == 's') {
            printf("\nSaindo do jogo a pedido do jogador. Ate logo!\n");
            break;
        } else {
            printf("[ERRO] Opção invalida. Por favor, escolha 'e', 'd' ou 's'.\n");
        }

        printf("\n"); // Espaçamento para a próxima iteração
    }
}

// ------------------------------------------
// 4. FUNÇÃO DE LIBERAÇÃO DE MEMÓRIA
// ------------------------------------------

/**
 * @brief Libera recursivamente a memória alocada para a árvore (mapa da mansão).
 *
 * Importante para evitar vazamento de memória.
 * @param sala O nó atual a ser liberado.
 */
void liberarMapa(Sala *sala) {
    if (sala != NULL) {
        liberarMapa(sala->esquerda);
        liberarMapa(sala->direita);
        // printf("Liberando: %s\n", sala->nome); // Opcional: para debug
        free(sala);
    }
}

// ------------------------------------------
// 5. FUNÇÃO PRINCIPAL
// ------------------------------------------

/**
 * @brief Monta o mapa inicial, dá início à exploração e gerencia a memória.
 */
int main() {
    // Monta a estrutura da mansão (a árvore binária)
    Sala *mapa = montarMapa();

    // Inicia a exploração interativa a partir da raiz (Hall de Entrada)
    explorarSalas(mapa);

    // Libera toda a memória alocada para o mapa após o término do jogo
    liberarMapa(mapa);
    
    printf("\nMemória do mapa liberada com sucesso.\n");
    return 0;
}