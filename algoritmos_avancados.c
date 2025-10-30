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
// 2. ESTRUTURAS DE DADOS
// ------------------------------------------

/**
 * @brief Estrutura que representa um nó da Árvore Binária de Busca (BST) de Pistas.
 *
 * Requisito: BST para armazenar e organizar as pistas.
 */
typedef struct PistaNode {
    char pista[MAX_PISTA];
    struct PistaNode *esquerda; // Pistas lexicograficamente menores
    struct PistaNode *direita;  // Pistas lexicograficamente maiores
} PistaNode;

/**
 * @brief Estrutura que representa um Cômodo (Nó) da mansão.
 *
 * Requisito: Cada cômodo tem nome e, opcionalmente, uma pista.
 */
typedef struct Sala {
    char nome[MAX_NOME];
    char pista[MAX_PISTA]; // Conteúdo da pista na sala (pode ser vazio)
    struct Sala *esquerda;  // Caminho à esquerda
    struct Sala *direita;   // Caminho à direita
} Sala;

// Ponteiro global para a BST de Pistas. 
// O mapa (Sala) é separado do registro de pistas (BST).
PistaNode *raiz_pistas = NULL;

// ------------------------------------------
// 3. FUNÇÕES DA BST (ÁRVORE DE PISTAS)
// ------------------------------------------

/**
 * @brief Cria e aloca um novo nó para a BST de Pistas.
 *
 * @param conteudo A string da pista a ser armazenada.
 * @return PistaNode* O ponteiro para o novo nó criado.
 */
PistaNode* criarPistaNode(const char *conteudo) {
    PistaNode *novoNo = (PistaNode*)malloc(sizeof(PistaNode));
    if (novoNo == NULL) {
        perror("Erro ao alocar memória para o nó da pista.");
        exit(EXIT_FAILURE);
    }
    strncpy(novoNo->pista, conteudo, MAX_PISTA - 1);
    novoNo->pista[MAX_PISTA - 1] = '\0';
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    return novoNo;
}

/**
 * @brief Insere uma nova pista na Árvore Binária de Busca (BST) de forma recursiva.
 *
 * Requisito: Armazenar as pistas coletadas. A BST garante a ordenação.
 * @param raiz O nó atual da BST.
 * @param conteudo A pista a ser inserida.
 * @return PistaNode* A nova raiz da subárvore após a inserção.
 */
PistaNode* inserirPista(PistaNode *raiz, const char *conteudo) {
    // Caso base: Se a subárvore estiver vazia, cria um novo nó e o retorna
    if (raiz == NULL) {
        return criarPistaNode(conteudo);
    }

    // Compara as strings para decidir o caminho (BST)
    int comparacao = strcmp(conteudo, raiz->pista);

    if (comparacao < 0) {
        // A nova pista é lexicograficamente MENOR: vai para a esquerda
        raiz->esquerda = inserirPista(raiz->esquerda, conteudo);
    } else if (comparacao > 0) {
        // A nova pista é lexicograficamente MAIOR: vai para a direita
        raiz->direita = inserirPista(raiz->direita, conteudo);
    } 
    // Se comparacao == 0, a pista é idêntica; não inserimos duplicatas (ignoramos).

    return raiz;
}

/**
 * @brief Exibe todas as pistas coletadas na BST em ordem alfabética (Percurso Em Ordem).
 *
 * Requisito: Exibir todas as pistas coletadas em ordem alfabética ao final.
 * @param raiz O nó atual da BST.
 */
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        // 1. Percorre a esquerda (menores)
        exibirPistas(raiz->esquerda); 
        
        // 2. Visita o nó atual (imprime a pista)
        printf("- %s\n", raiz->pista); 
        
        // 3. Percorre a direita (maiores)
        exibirPistas(raiz->direita);
    }
}

/**
 * @brief Libera recursivamente a memória alocada para a BST de pistas.
 */
void liberarPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
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