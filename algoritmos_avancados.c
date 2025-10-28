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