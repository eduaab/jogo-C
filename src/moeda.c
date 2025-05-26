#include "moeda.h"
#include "fixo.h"
#include "game.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

float tempoVidaMoeda = 13.0f;

#define MOEDA_LARGURA 27
#define MOEDA_ALTURA 26

// Declaração das texturas das moedas (global)
Texture2D moedaPrataTex;  // Textura da moeda de prata
Texture2D moedaOuroTex;   // Textura da moeda de ouro

// Carrega o som da moeda para a memória
void CarregarSonsMoedas(void) {
    somMoeda = LoadSound("audio/coinCatch.wav");  // Som tocado ao coletar uma moeda
}

// Libera o som da memória quando não for mais usado
void DescarregarSonsMoedas(void) {
    UnloadSound(somMoeda);  // Libera o recurso de som
}

// Carrega as imagens das moedas
void CarregarTexturasMoedas(void) {
    moedaPrataTex = LoadTexture("sprites/png/moeda_prata.png"); // Imagem da moeda prata
    moedaOuroTex  = LoadTexture("sprites/png/moeda_ouro.png");  // Imagem da moeda ouro
}

// Libera as texturas da memória
void UnloadTexturasMoedas(void) {
    UnloadTexture(moedaPrataTex); // Libera textura da moeda prata
    UnloadTexture(moedaOuroTex);  // Libera textura da moeda ouro
}

// Inicializa o vetor de moedas com valores padrões
void InicializarMoedas(Moeda moedas[]) {
    srand(time(NULL));  // Inicializa gerador de números aleatórios com base no tempo

    // Para cada moeda do array
    for (int i = 0; i < MAX_MOEDAS; i++) {
        moedas[i].ativa        = false;                 // Moeda começa desativada (não visível)
        moedas[i].posicao      = (Vector2){0, 0};       // Posição inicial (fora da tela)
        moedas[i].tipo         = PRATA;                 // Tipo padrão é PRATA
        moedas[i].tempoVida    = 0.0f;                  // Tempo de vida atual começa em 0
        moedas[i].duracaoVida  = 0.0f;                  // Duração total de vida será definida depois
    }
}

// Atualiza o tempo de vida das moedas com base no tempo total do jogo
void AtualizarTempoVidaMoeda(float tempoTotalJogo) {
    float novaVida = 13.0f - floorf(tempoTotalJogo / 30.0f); // A cada 30s, diminui 1s da vida
    tempoVidaMoeda = fmaxf(5.0f, novaVida);                  // Mas o mínimo é 5 segundos
}

// Função que gerencia a criação, ativação e expiração das moedas
void AtualizarMoedas(Moeda moedas[], float *tempoRespawn, float tempoTotalJogo,  Rectangle areaJogo) {
    AtualizarTempoVidaMoeda(tempoTotalJogo);        // Atualiza tempo de vida das moedas baseado no tempo de jogo
    *tempoRespawn += GetFrameTime();                // Soma o tempo desde o último frame ao tempo de respawn

    // Se passou mais de 1.5 segundos desde a última tentativa de spawn
    if (*tempoRespawn >= 1.5f) {
        *tempoRespawn = 0;  // Zera o contador de respawn

        // Procura uma moeda inativa para ativar
        for (int i = 0; i < MAX_MOEDAS; i++) {
            if (!moedas[i].ativa) {
                // 70% de chance de ativar uma moeda
                if (rand() % 100 < 70) {
                    moedas[i].ativa = true;

                    // Gera posição aleatória dentro da área do jogo
                    moedas[i].posicao = (Vector2){
                        rand() % (int)(areaJogo.width - 32) + (int)areaJogo.x,
                        rand() % (int)(areaJogo.height - 32) + (int)areaJogo.y
                    };

                    // 80% de chance de ser PRATA, 20% OURO
                    if (rand() % 100 < 80) {
                        moedas[i].tipo = PRATA;
                    } else {
                        moedas[i].tipo = OURO;
                    }

                    // Inicializa tempos de vida
                    moedas[i].tempoVida   = 0.0f;
                    moedas[i].duracaoVida = tempoVidaMoeda;

                    break; // Ativou uma moeda, sai do laço
                }
            }
        }
    }

    // Atualiza o tempo de vida de cada moeda ativa
    for (int i = 0; i < MAX_MOEDAS; i++) {
        if (moedas[i].ativa) {
            moedas[i].tempoVida += GetFrameTime();  // Incrementa tempo de vida da moeda

            // Se o tempo de vida passou da duração, desativa a moeda
            if (moedas[i].tempoVida >= moedas[i].duracaoVida){
                moedas[i].ativa = false;
            } 
        }
    }
}

// Desenha as moedas na tela
void DesenharMoedas(Moeda moedas[]) {
    for (int i = 0; i < MAX_MOEDAS; i++) {
        if (!moedas[i].ativa) continue;  // Ignora moedas inativas

        Texture2D tex;

        // Define qual textura usar com base no tipo da moeda
        if (moedas[i].tipo == PRATA) {
            tex = moedaPrataTex;
        } else {
            tex = moedaOuroTex;
        }

        Color cor = WHITE; // Cor padrão: branco (sem filtro)

        // Calcula quanto tempo ainda falta para a moeda desaparecer
        float tempoRestante = moedas[i].duracaoVida - moedas[i].tempoVida;

        // Se faltar menos de 3 segundos, a moeda começa a piscar
        if (tempoRestante <= 3.0f) {
            float piscar = fmodf(GetTime(), 0.4f);  // Oscila entre 0.0 e 0.4
            if (piscar < 0.3f) cor.a = 100;         // Torna a moeda semitransparente (piscar)
        }

        // Desenha a moeda com tamanho diferente dependendo do tipo
        
        // Desenha a moeda com tamanho diferente dependendo do tipo
        if (moedas[i].tipo == OURO){
            DrawTextureEx(tex, moedas[i].posicao, 0.0f, 1.2f, cor); // OURO maior (escala 1.2)
        }else{
            DrawTextureV(tex, moedas[i].posicao, cor);              // PRATA tamanho normal
        }
    }
}


// Define as dimensões padrão da hitbox do jogador (não estão sendo usadas diretamente aqui,
// mas podem servir como referência futura)
#define JOGADOR_HITBOX_LARGURA 16
#define JOGADOR_HITBOX_ALTURA  20

// Função que retorna a hitbox do jogador com base na posição atual e no tamanho dos sprites
Rectangle getHitboxJogador(Jogador *jogador) {
    return (Rectangle){
        jogador->posicao.x,                // Posição X do jogador na tela
        jogador->posicao.y,                // Posição Y do jogador na tela
        jogador->direita[0].width,         // Largura baseada no sprite "andando para a direita"
        jogador->baixo[0].height           // Altura baseada no sprite "andando para baixo"
    };
}

// Função que detecta colisão entre o jogador e as moedas, e processa a coleta
void colisaoMoedas(Moeda moedas[], Jogador *jogador, Sound somMoeda) {
    // Calcula a hitbox do jogador
    Rectangle hitboxJogador = getHitboxJogador(jogador);

    // Percorre todas as moedas
    for (int i = 0; i < MAX_MOEDAS; i++) {
        // Se a moeda não está ativa (invisível ou já coletada), ignora
        if (!moedas[i].ativa) continue;

        // Define a hitbox da moeda, centralizando em relação à sua posição
        Rectangle rectMoeda = {
            moedas[i].posicao.x - MOEDA_LARGURA / 2.0f,  // Corrige posição X para centralizar
            moedas[i].posicao.y - MOEDA_ALTURA / 2.0f,   // Corrige posição Y para centralizar
            MOEDA_LARGURA,                               // Largura da moeda
            MOEDA_ALTURA                                 // Altura da moeda
        };

        // Verifica se houve colisão entre a hitbox do jogador e a da moeda
        if (CheckCollisionRecs(hitboxJogador, rectMoeda)) {

            // Se a moeda for de OURO, incrementa o contador de moedas de ouro
            if (moedas[i].tipo == OURO) {
                jogador->moedasOuro++;
            } else {
                // Caso contrário, incrementa o contador de moedas de prata
                jogador->moedasPrata++;
            }

            // Toca o som de coleta da moeda
            PlaySound(somMoeda);

            // Desativa a moeda para que ela não possa ser coletada novamente
            moedas[i].ativa = false;

            // Define o tempo até o qual um texto ("+1") ou efeito visual será exibido
            jogador->tempoTextoMoeda = GetTime() + 0.2f;
        }
    }
}




