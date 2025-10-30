#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ------------------------------------------
// 1. CONSTANTES E DEFINIÇÕES
// ------------------------------------------
#define MAX_NOME 50
#define MAX_PISTA 100
#define TAMANHO_HASH 7 // Tamanho primo para a Tabela Hash
#define PISTAS_MINIMAS 2 // Requisito: Pelo menos 2 pistas para sustentar a acusacao

// ------------------------------------------
// 2. ESTRUTURAS DA TABELA HASH (Suspeitos por Pista)
// ------------------------------------------

/**
 * @brief Estrutura de um Nó de Lista Ligada para o Encademento da Hash.
 *
 * Armazena a pista (chave) e o suspeito (valor) associado.
 */
typedef struct HashNode {
    char pista[MAX_PISTA];
    char suspeito[MAX_NOME];
    struct HashNode *proximo;
} HashNode;

/**
 * @brief Estrutura da Tabela Hash usando Encademento Separado.
 *
 * Cada índice do vetor aponta para uma lista ligada (HashNode).
 */
typedef struct {
    HashNode *tabela[TAMANHO_HASH];
} TabelaHash;

// ------------------------------------------
// 3. ESTRUTURAS DA BST (Pistas Coletadas)
// ------------------------------------------

/**
 * @brief Estrutura que representa um nó da Árvore Binária de Busca (BST) de Pistas.
 */
typedef struct PistaNode {
    char pista[MAX_PISTA];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// ------------------------------------------
// 4. FUNÇÕES DO MAPA (ÁRVORE BINÁRIA)
// ------------------------------------------

/**
 * @brief Cria e inicializa um novo cômodo (nó) da mansão.
 *
 * Requisito: Cria dinamicamente um cômodo com nome e pista.
 * @param nome O nome do cômodo.
 * @param pista O conteúdo da pista (ou string vazia se não houver).
 * @return Sala* Um ponteiro para a nova sala criada.
 */
Sala* criarSala(const char *nome, const char *pista) {
    // Aloca dinamicamente o espaço para a nova sala
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));

    if (novaSala == NULL) {
        perror("Erro ao alocar memória para a sala.");
        exit(EXIT_FAILURE);
    }

    // Inicializa campos
    strncpy(novaSala->nome, nome, MAX_NOME - 1);
    novaSala->nome[MAX_NOME - 1] = '\0';
    
    strncpy(novaSala->pista, pista, MAX_PISTA - 1);
    novaSala->pista[MAX_PISTA - 1] = '\0';

    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

/**
 * @brief Constrói o mapa fixo da mansão como uma Árvore Binária, com pistas.
 *
 * Requisito: O mapa é fixo e pré-definido.
 * @return Sala* O ponteiro para o nó Raiz (Hall de Entrada).
 */
Sala* montarMapa() {
    // A cada sala, associamos uma pista. Se a string for vazia (""), não há pista.
    
    // Nível 0: Raiz
    Sala *hall = criarSala("Hall de Entrada", "O culpado tem medo de alturas.");

    // Nível 1
    hall->esquerda = criarSala("Sala de Estar", ""); // Sem pista aqui
    hall->direita = criarSala("Cozinha", "Uma faca de prata sumiu.");

    // Nível 2
    hall->esquerda->esquerda = criarSala("Quarto Principal", "O mordomo usava luvas."); // Esquerda da Sala de Estar
    hall->esquerda->direita = criarSala("Biblioteca", "Um bilhete rasgado esta sob o tapete."); // Direita da Sala de Estar

    hall->direita->direita = criarSala("Dispensa", "Cheiro forte de querosene."); // Direita da Cozinha
    
    // Nível 3 (Folhas)
    hall->esquerda->esquerda->esquerda = criarSala("Banheiro", "Uma pegada enlameada foi deixada."); // Esquerda do Quarto Principal
    hall->esquerda->direita->direita = criarSala("Jardim", ""); // Sem pista

    return hall;
}

// ------------------------------------------
// 5. FUNÇÃO DE EXPLORAÇÃO E COLETA
// ------------------------------------------

/**
 * @brief Controla a navegação do jogador, exibe informações e coleta pistas.
 *
 * Requisito: Adicionar automaticamente à BST cada pista encontrada.
 * @param raiz O nó raiz da árvore (Hall de Entrada).
 */
void explorarSalasComPistas(Sala *raiz) {
    Sala *atual = raiz;
    char opcao;
    
    printf("\n>>> BEM-VINDO(A) ao Detective Quest - Coleta de Pistas! <<<\n");
    printf("Explore a mansao e encontre as pistas essenciais.\n\n");

    // Loop de exploração: continua até o usuário decidir 's' (sair)
    while (atual != NULL) {
        printf("========================================================\n");
        printf("VOCE ESTA EM: %s\n", atual->nome);

        // Lógica de coleta de pista
        if (atual->pista[0] != '\0') {
            // Verifica se a pista já foi coletada (usando a BST como referência)
            // Para simplificação, vamos apenas inserir, e a BST gerencia duplicatas.
            
            // Requisito: Adicionar automaticamente à árvore de pistas.
            raiz_pistas = inserirPista(raiz_pistas, atual->pista);
            
            printf("[PISTA ENCONTRADA] Coletado: \"%s\"\n", atual->pista);
            // Zera a pista da sala para que não seja coletada novamente
            atual->pista[0] = '\0'; 
        } else {
            printf("[INFO] Nenhuma pista foi encontrada neste comodo (ou ja foi coletada).\n");
        }
        
        printf("--------------------------------------------------------\n");
        printf("Opcoes de caminho:\n");

        int caminhos_disponiveis = 0;
        if (atual->esquerda != NULL) {
            printf(" [E] Esquerda: %s\n", atual->esquerda->nome);
            caminhos_disponiveis = 1;
        }
        if (atual->direita != NULL) {
            printf(" [D] Direita: %s\n", atual->direita->nome);
            caminhos_disponiveis = 1;
        }

        if (!caminhos_disponiveis) {
            printf("[FIM DE CAMINHO] Nenhuma saida adicional. Voce pode apenas sair.\n");
        }

        printf(" [S] Sair do Jogo e Analisar Pistas.\n");
        printf("--------------------------------------------------------\n");
        printf("Sua escolha (e/d/s): ");

        // Lendo a opção do usuário
        if (scanf(" %c", &opcao) != 1) {
            while (getchar() != '\n');
            opcao = 's'; // Assume sair em caso de entrada inválida
        }
        opcao = tolower(opcao);

        // Controle das decisões
        if (opcao == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("[ALERTA] Nao ha caminho a esquerda a partir daqui.\n");
            }
        } else if (opcao == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("[ALERTA] Nao ha caminho a direita a partir daqui.\n");
            }
        } else if (opcao == 's') {
            printf("\nSaindo da exploracao para analise final...\n");
            break;
        } else {
            printf("[ERRO] Opcao invalida. Por favor, escolha 'e', 'd' ou 's'.\n");
        }
        printf("\n");
    }
}

/**
 * @brief Libera recursivamente a memória alocada para o mapa da mansão.
 */
void liberarMapa(Sala *sala) {
    if (sala != NULL) {
        liberarMapa(sala->esquerda);
        liberarMapa(sala->direita);
        free(sala);
    }
}

// ------------------------------------------
// 6. FUNÇÃO PRINCIPAL
// ------------------------------------------

int main() {
    // 1. Monta o Mapa Fixo da Mansão
    Sala *mapa = montarMapa();

    // 2. Inicia a Exploração e Coleta de Pistas
    explorarSalasComPistas(mapa);
    
    // 3. Exibe o resultado final (Pistas Ordenadas)
    printf("\n********************************************************\n");
    printf("        RELATORIO FINAL: PISTAS COLETADAS\n");
    printf("        (Organizadas em Ordem Alfabetica)\n");
    printf("********************************************************\n");
    
    if (raiz_pistas == NULL) {
        printf("Nenhuma pista foi coletada durante a exploracao.\n");
    } else {
        exibirPistas(raiz_pistas);
    }
    printf("********************************************************\n");

    // 4. Limpeza de Memória
    liberarMapa(mapa);
    liberarPistas(raiz_pistas);
    
    printf("\nSistema encerrado e memoria liberada.\n");
    return 0;
}