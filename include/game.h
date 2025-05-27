#ifndef JOGADOR_H
#define JOGADOR_H

#define MAX_TEXTS 50  // Número máximo de textos que podem ser exibidos (por exemplo, mensagens de perda de vida)

#include "raylib.h"  // Biblioteca gráfica para o jogo (texturas, vetores, desenho, etc)
#include "fixo.h"    // Definições fixas do projeto (por exemplo, constantes de frames)
#include <stdbool.h> // Para usar tipo booleano 'bool'

// Enumeração para definir as direções que o jogador pode estar olhando ou se movendo
typedef enum { UP, DOWN, LEFT, RIGHT } Direction;

// Estrutura para gerenciar textos de perda que aparecem na tela (ex: "-10 de vida")
typedef struct TextoPerda {
    bool active;          // Indica se o texto está ativo (deve ser desenhado)
    Vector2 posicao;      // Posição do texto na tela
    float Congelado;      // Tempo até o texto desaparecer (tempo congelado)
    char texto[32];       // Texto a ser exibido (máximo 32 caracteres)
} TextoPerda;

// Estrutura principal do jogador, com suas propriedades e sprites
typedef struct {
    Vector2  posicao;               // Posição atual do jogador no mapa
    float    velocidade;            // Velocidade do jogador em unidades por frame
    Direction direcaoAtual;         // Direção que o jogador está olhando (UP, DOWN, LEFT, RIGHT)
    float    tempoFrame;            // Tempo que cada frame da animação deve durar (em segundos)
    float    temporizador;          // Temporizador para controle de troca de frame na animação
    int      indiceFrame;           // Índice do frame atual da animação
    int      moedasPrata;           // Contador de moedas prata coletadas
    int      moedasOuro;            // Contador de moedas ouro coletadas
    float    tempoUltimaColisaoGhost2; // Guarda o tempo da última colisão com um Ghost2 para controle de invencibilidade
    float    tempoTextoMoeda;       // Tempo para controle de exibição de texto de moeda (ex: "+1 moeda")
    Texture2D cima   [MAX_FRAMES_CIMA_BAIXO];    // Array de texturas para animação de movimento para cima
    Texture2D baixo  [MAX_FRAMES_CIMA_BAIXO];    // Array de texturas para animação de movimento para baixo
    Texture2D esquerda[MAX_FRAMES_LADO];          // Array de texturas para animação de movimento para esquerda
    Texture2D direita [MAX_FRAMES_LADO];          // Array de texturas para animação de movimento para direita
} Jogador;

// Função para criar o jogador, lendo os sprites do JSON, e inicializando posição e estados iniciais
Jogador CriarJogador(const char *jsonPath, const char *nomeNoJson, Vector2 posicaoInicial);

// Atualiza a posição do jogador com base nas teclas pressionadas e dentro dos limites da área de jogo
void AtualizarJogador(Jogador *j, int upKey, int downKey, int leftKey, int rightKey, Rectangle areaJogo);

// Atualiza a animação do jogador (muda os frames) dependendo se ele está andando ou parado
void AtualizarAnimacao(Jogador *j, int andando);

// Desenha o jogador na tela usando a textura correspondente à direção e frame atual
void DesenharJogador(Jogador *j);

// Libera as texturas carregadas do jogador da memória
void DestruirJogador(Jogador *j);

#endif
