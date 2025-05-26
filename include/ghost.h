#ifndef GHOST_H
#define GHOST_H

#include "raylib.h"   // Biblioteca para gráficos, entrada e manipulação de texturas, sons, etc.
#include "cJSON.h"    // Biblioteca para manipular arquivos JSON (usada para carregar dados dos fantasmas).
#include "game.h"     // Cabeçalho do jogo que define estruturas como Jogador, TextoPerda e enums.


#define MAX_GHOST_FRAMES 10  
#define MAX_FANTASMAS 50

typedef struct {
    Vector2 posicao;             // Posição atual do fantasma na tela (x, y).
    float velocidade;            // Velocidade de movimento do fantasma.
    Direction currentDirection;  // Direção em que o fantasma está se movendo (UP, DOWN, LEFT, RIGHT).
    float frameTime;             // Tempo entre trocas de quadros da animação.
    float timer;                 // Cronômetro que controla quando trocar o quadro.
    int frameIndex;              // Índice atual do quadro sendo exibido.

    float moveLimite;            // Tempo até o fantasma mudar de direção automaticamente.
    float moveTimer;             // Cronômetro interno para medir o tempo atual de movimento.

    Texture2D up[MAX_GHOST_FRAMES];    // Animação para cima.
    Texture2D down[MAX_GHOST_FRAMES];  // Animação para baixo.
    Texture2D left[MAX_GHOST_FRAMES];  // Animação para esquerda.
    Texture2D right[MAX_GHOST_FRAMES]; // Animação para direita.

    int frameCount[4];           // Quantidade de quadros em cada direção (índices: UP=0, DOWN=1, LEFT=2, RIGHT=3).
} Ghost;


typedef enum { MOVENDO_X, MOVENDO_Y } EixoMovimento; // Define se o Ghost2 está se movendo priorizando o eixo X ou o eixo Y. Ele troca isso com base na posição do jogador.

typedef struct {
    Ghost ghost;                 // Herda todas as propriedades do Ghost normal.
    float tempoTextoMoeda;      // Tempo restante para exibir o texto "-1 moeda".
    float velocidade;           // Velocidade atual do Ghost2.
    float tempoEixo;            // Tempo restante até poder trocar o eixo de perseguição.
    EixoMovimento eixoAtual;    // Qual eixo está sendo usado para perseguir (X ou Y).
} Ghost2;



typedef struct {
    Ghost fantasmas[MAX_FANTASMAS]; // Vetor contendo os fantasmas ativos.
    int quantidade;                 // Quantidade atual de fantasmas na lista.
    float tempoDesdeUltimo;        // Tempo desde que o último fantasma foi criado.
} ListaFantasmas;


void InicializarListaFantasmas(ListaFantasmas *lista, const char *caminhoJSON, const char *chaveFantasma);
void AtualizarListaFantasmas(ListaFantasmas *lista, Vector2 jogadorPos, Rectangle areaJogo, float delta);
void DesenharListaFantasmas(ListaFantasmas *lista);
void DestruirListaFantasmas(ListaFantasmas *lista);
Ghost CriarFantasma(const char *caminhoJSON, const char *chaveFantasma, Vector2 posicaoInicial);
Ghost2 CriarGhost2(const char *caminhoJSON, const char *chaveFantasma, Vector2 posicao);
void AtualizarFantasma(Ghost *g, Vector2 jogadorPos, Rectangle areaJogo);
void DesenharFantasma(Ghost *g);
void DestruirFantasma(Ghost *g);
bool VerificarColisaoFantasma(Ghost *g, Jogador *j, Sound somSusto);
void VerificarColisaoGhost2(Ghost2 *ghost, Jogador *j, Sound somSusto, TextoPerda textos[MAX_TEXTS]);
void DesenharTextosPerda(TextoPerda textos[MAX_TEXTS]);
void CarregarSomFantasma(void);
void DescarregarSomFantasma(void);

void AtualizarGhost2(Ghost2 *Ghost, Vector2 jogadorPos, Rectangle areaJogo);
void VerificarColisaoGhost2(Ghost2 *ghost, Jogador *j, Sound somColisao, TextoPerda textos[MAX_TEXTS]);

#endif
