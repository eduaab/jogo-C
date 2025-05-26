#ifndef MENU_H
#define MENU_H

#include "raylib.h"  // Biblioteca para manipulação gráfica, teclado, janelas, etc.
#include "game.h"    // Provavelmente contém definições e estruturas do jogo, incluindo TelaAtual

// Declaração da função que atualiza a seleção do menu principal
// Recebe um retângulo de botão (para possíveis interações), um ponteiro para a tela atual, e um ponteiro para a opção selecionada
void AtualizarMenu(Rectangle botao, TelaAtual *telaAtual, int *opcao);

// Declaração da função que desenha o menu principal na tela
// Recebe o retângulo do botão, a textura do background e a opção selecionada para destacar visualmente
void DesenharMenu(Rectangle botao, Texture2D background, int opcao);

#endif
