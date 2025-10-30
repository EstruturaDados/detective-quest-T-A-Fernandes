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
// 4. ESTRUTURA DO MAPA (Árvore Binária)
// ------------------------------------------

/**
 * @brief Estrutura que representa um Cômodo (Nó) da mansão.
 */
typedef struct Sala {
    char nome[MAX_NOME];
    char pista_estatica[MAX_PISTA]; // Pista original associada à sala
    int pista_coletada;             // Flag: 1 se coletada, 0 caso contrário
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Ponteiros globais
PistaNode *raiz_pistas = NULL;
TabelaHash tabela_suspeitos; 


// ------------------------------------------
// 5. FUNÇÕES DA TABELA HASH
// ------------------------------------------

/**
 * @brief Inicializa todos os baldes (buckets) da Tabela Hash como NULL.
 */
void inicializarHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabela_suspeitos.tabela[i] = NULL;
    }
}

/**
 * @brief Função Hash simples (Soma dos ASCII mod TAMANHO_HASH).
 *
 * @param chave A string (pista) a ser mapeada.
 * @return int O índice na tabela.
 */
int calcularHash(const char *chave) {
    int hash = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        hash = (hash + (int)chave[i]);
    }
    return hash % TAMANHO_HASH;
}

/**
 * @brief Insere a associação Pista (Chave) -> Suspeito (Valor) na Tabela Hash.
 *
 * Requisito: Inserir associação pista/suspeito na tabela hash.
 * @param pista A chave (pista) a ser inserida.
 * @param suspeito O valor (suspeito) associado à pista.
 */
void inserirNaHash(const char *pista, const char *suspeito) {
    int indice = calcularHash(pista);
    HashNode *novoNo = (HashNode*)malloc(sizeof(HashNode));
    
    if (novoNo == NULL) {
        perror("Erro ao alocar memoria para HashNode.");
        exit(EXIT_FAILURE);
    }
    
    // Configura o novo nó
    strncpy(novoNo->pista, pista, MAX_PISTA - 1);
    strncpy(novoNo->suspeito, suspeito, MAX_NOME - 1);
    novoNo->proximo = NULL;

    // Encademento Separado: Insere no início da lista do balde
    novoNo->proximo = tabela_suspeitos.tabela[indice];
    tabela_suspeitos.tabela[indice] = novoNo;
    
    printf("  [HASH REGISTRO] Pista associada a '%s' (indice %d).\n", suspeito, indice);
}

/**
 * @brief Consulta a Tabela Hash para encontrar o suspeito associado a uma pista.
 *
 * Requisito: Consultar o suspeito correspondente a uma pista.
 * @param pista A chave (pista) a ser procurada.
 * @return char* O nome do suspeito, ou "DESCONHECIDO" se não encontrado.
 */
const char* encontrarSuspeito(const char *pista) {
    int indice = calcularHash(pista);
    HashNode *atual = tabela_suspeitos.tabela[indice];
    
    // Percorre a lista ligada (Encadeamento)
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito; // Suspeito encontrado
        }
        atual = atual->proximo;
    }
    
    return "DESCONHECIDO"; // Pista sem associação na Hash
}

/**
 * @brief Libera a memória alocada para a Tabela Hash.
 */
void liberarHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        HashNode *atual = tabela_suspeitos.tabela[i];
        while (atual != NULL) {
            HashNode *temp = atual;
            atual = atual->proximo;
            free(temp);
        }
        tabela_suspeitos.tabela[i] = NULL;
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