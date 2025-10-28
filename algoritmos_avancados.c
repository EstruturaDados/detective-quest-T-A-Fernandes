#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------------------------
// 1. ESTRUTURAS DE DADOS
// ------------------------------------------

/**
 * @brief Estrutura que representa um Cômodo (Nó) da mansão.
 *
 * Requisito: Cada cômodo deve ter um nome e a estrutura de uma árvore binária.
 */
typedef struct Sala {
    char nome[50];       // Nome do cômodo (ex: Hall, Cozinha)
    struct Sala *esquerda;  // Ponteiro para o cômodo à esquerda (Filho Esquerdo)
    struct Sala *direita;   // Ponteiro para o cômodo à direita (Filho Direito)
} Sala;
