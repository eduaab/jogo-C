// Proteção contra inclusões múltiplas
#ifndef COIN_H
#define COIN_H

// Inclusão das bibliotecas necessárias
#include "raylib.h"      // Biblioteca gráfica para manipulação de texturas e sons
#include "game.h"        // Estruturas e definições relacionadas ao jogador e jogo

// Número máximo de moedas que podem existir ao mesmo tempo na tela
#define MAX_MOEDAS 20

// Enumeração para os dois tipos de moedas possíveis: prata e ouro
typedef enum { PRATA, OURO } TipoMoeda;

// Estrutura que define uma moeda no jogo
typedef struct {
    Vector2 posicao;       // Posição da moeda na tela
    bool ativa;            // Define se a moeda está ativa (visível e interagível)
    TipoMoeda tipo;        // Tipo da moeda (PRATA ou OURO)
    float tempoVida;       // Tempo que a moeda já ficou ativa
    float duracaoVida;     // Tempo total que a moeda pode ficar ativa antes de desaparecer
} Moeda;

// Texturas das moedas, declaradas como variáveis globais
extern Texture2D moedaPrataTex;  // Textura da moeda de prata
extern Texture2D moedaOuroTex;   // Textura da moeda de ouro

// Som tocado ao coletar uma moeda
extern Sound somMoeda;

// Funções relacionadas à lógica das moedas no jogo

// Inicializa todas as moedas (desativa e zera os dados)
void InicializarMoedas(Moeda moedas[]);

// Atualiza o estado das moedas (spawn, tempo de vida, etc)
void AtualizarMoedas(Moeda moedas[], float *tempoRespawn, float tempoTotalJogo, Rectangle areaJogo);

// Desenha as moedas ativas na tela
void DesenharMoedas(Moeda moedas[]);

// Carrega as texturas das moedas a partir dos arquivos
void CarregarTexturasMoedas(void);

// Libera a memória das texturas quando não são mais necessárias
void UnloadTexturasMoedas(void);

// Carrega o som usado quando uma moeda é coletada
void CarregarSonsMoedas(void);

// Libera o som da memória
void DescarregarSonsMoedas(void);

// Verifica colisão entre o jogador e moedas, aplica efeitos e coleta
void colisaoMoedas(Moeda moedas[], Jogador *jogador, Sound somMoeda);

#endif // Fim da proteção contra inclusões múltiplas
