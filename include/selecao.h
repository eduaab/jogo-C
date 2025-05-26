#ifndef SELECAO_H
#define SELECAO_H

#include "raylib.h"   // Biblioteca para manipulação gráfica, teclado, etc.
#include "fixo.h"     // Provavelmente contém definições fixas do jogo, como enum TelaAtual

// Declaração da função que atualiza a seleção do número de jogadores
// Recebe dois botões (para futuro uso possivelmente), um ponteiro para a tela atual e um ponteiro para a opção selecionada
void AtualizarSelecaoJogadores(Rectangle botao1, Rectangle botao2, TelaAtual *tela, int *opcao);

// Declaração da função que desenha a tela de seleção de jogadores
// Recebe dois botões, uma textura de background e a opção selecionada para desenhar a interface corretamente
void DesenharSelecaoJogadores(Rectangle botao1, Rectangle botao2, Texture2D background, int opcao);

// Declaração da função que atualiza a tela "Sobre o Jogo"
// Modifica a tela atual de acordo com a entrada do usuário (por exemplo, voltar ao menu)
void AtualizarSobre(TelaAtual *tela);

// Declaração da função que desenha a tela "Sobre o Jogo" com um background fornecido
void DesenharSobre(Texture2D background);

#endif
