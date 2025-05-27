#ifndef CONSTANTS_H
#define CONSTANTS_H

// Inclusão da biblioteca Raylib para manipulação gráfica e do tipo Rectangle
#include <raylib.h>  

// Inclusão da biblioteca cJSON para manipulação de JSON
#include "cJSON.h"   

// --- Definições de constantes para o jogo ---

// Largura da janela do jogo em pixels
#define WINDOW_WIDTH  1280

// Altura da janela do jogo em pixels
#define WINDOW_HEIGHT  720

// Número máximo de frames para animações verticais (cima e baixo)
// Usado para controlar spritesheets com animações de até 6 frames verticais
#define MAX_FRAMES_CIMA_BAIXO 6

// Número máximo de frames para animações horizontais (lado)
// Usado para controlar spritesheets com animações de até 4 frames horizontais
#define MAX_FRAMES_LADO       4

// Define os limites da área jogável ou de movimentação dentro da janela
#define LIMITE_X 100            // Posição X inicial do limite
#define LIMITE_Y 100            // Posição Y inicial do limite
#define LIMITE_LARGURA 800      // Largura da área limitada
#define LIMITE_ALTURA 500       // Altura da área limitada

// --- Definição de um tipo enumerado para controlar as telas do jogo ---

/**
 * Enumeração que representa as diferentes telas ou estados do jogo.
 * Pode ser usado para controlar a lógica do que está sendo exibido no momento.
 */
typedef enum { 
    MENU,       // Tela principal de menu
    SELECAO,    // Tela de seleção (exemplo: seleção de personagem ou fase)
    SOBRE,      // Tela com informações "sobre" o jogo ou desenvolvedores
    RANKING,    // Tela de ranking ou placar de jogadores
    JOGO        // Tela principal do jogo em si, onde o gameplay ocorre
} TelaAtual;

// Declaração externa da variável 'areaJogo', que é um retângulo definido em outro arquivo .c
// Representa a área do jogo onde elementos importantes estarão posicionados ou desenhados
extern Rectangle areaJogo;

// Protótipo da função para carregar texturas a partir de um array JSON
// - imagens: vetor onde as texturas carregadas serão armazenadas
// - array: objeto JSON contendo uma lista de strings (caminhos das imagens)
// - quantidade: ponteiro para inteiro onde será armazenada a quantidade de texturas carregadas
void CarregarTexturas(Texture2D *imagens, cJSON *array, int *quantidade);

#endif
