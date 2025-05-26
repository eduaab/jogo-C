#include "fixo.h"
#include "ghost.h"
#include "cJSON.h"
#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

// Função que cria um fantasma com base nos dados de um arquivo JSON
Ghost CriarFantasma(const char *caminhoJSON, const char *chaveFantasma, Vector2 posicaoInicial) {
    Ghost g = {0}; // Inicializa o struct com zero
    g.posicao = posicaoInicial;
    g.velocidade = 4;
    g.currentDirection = DOWN;
    g.frameTime = 0.2f;  // Tempo entre quadros de animação
    g.moveLimite = 1.5f; // Tempo que o fantasma leva para mudar de direção

    // Abre o arquivo JSON
    FILE *f = fopen(caminhoJSON, "rb");
    if (f == NULL){
        return g;
    }

    // Determina o tamanho do arquivo
    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    rewind(f);

    // Lê o conteúdo do arquivo para um buffer
    char *buffer = malloc(tamanho + 1);
    if (buffer == NULL) {
        fclose(f);
        return g;
    }

    size_t lidos = fread(buffer, 1, tamanho, f);
    fclose(f);

    if (lidos != tamanho) {
        free(buffer);
        return g;
    }

    buffer[tamanho] = '\0'; // Adiciona caractere nulo ao final da string

    // Faz o parsing do JSON
    cJSON *root = cJSON_Parse(buffer);
    free(buffer);
    if (root == NULL){
        return g;
    }

    // Procura pela chave do fantasma dentro do JSON
    cJSON *obj = cJSON_GetObjectItem(root, chaveFantasma);
    if (obj == NULL) {
        cJSON_Delete(root);
        return g;
    }

    // Carrega as animações (texturas) de cada direção
    CarregarTexturas(g.up,    cJSON_GetObjectItem(obj, "up"),    &g.frameCount[UP]);
    CarregarTexturas(g.down,  cJSON_GetObjectItem(obj, "down"),  &g.frameCount[DOWN]);
    CarregarTexturas(g.left,  cJSON_GetObjectItem(obj, "left"),  &g.frameCount[LEFT]);
    CarregarTexturas(g.right, cJSON_GetObjectItem(obj, "right"), &g.frameCount[RIGHT]);

    cJSON_Delete(root);

    return g;
}

// Atualiza a posição, direção e animação do fantasma
void AtualizarFantasma(Ghost *g, Vector2 jogadorPos, Rectangle areaJogo) {
    float dt = GetFrameTime(); // Tempo entre frames
    g->moveTimer += dt;        // Timer para mudar de direção
    g->timer += dt;            // Timer da animação

    // Se passou tempo suficiente, muda a direção do fantasma aleatoriamente
    if (g->moveTimer >= g->moveLimite) {
        g->currentDirection = GetRandomValue(0, 3); // 0 a 3: UP, DOWN, LEFT, RIGHT
        g->moveTimer = 0;
    }

    // Pega largura e altura do sprite atual
    float largura = g->right[0].width;
    float altura = g->down[0].height;

    // Move o fantasma conforme sua direção atual, respeitando os limites da área de jogo
    switch (g->currentDirection) {
        case UP:
            if (g->posicao.y > areaJogo.y)
                g->posicao.y -= g->velocidade;
            break;
        case DOWN:
            if (g->posicao.y + altura < areaJogo.y + areaJogo.height)
                g->posicao.y += g->velocidade;
            break;
        case LEFT:
            if (g->posicao.x > areaJogo.x)
                g->posicao.x -= g->velocidade;
            break;
        case RIGHT:
            if (g->posicao.x + largura < areaJogo.x + areaJogo.width)
                g->posicao.x += g->velocidade;
            break;
    }

    // Atualiza o índice do quadro de animação, se houver mais de 1
    int totalFrames = g->frameCount[g->currentDirection];
    if (totalFrames > 1 && g->timer >= g->frameTime) {
        g->frameIndex = (g->frameIndex + 1) % totalFrames;
        g->timer = 0;
    }
}

// Desenha o fantasma na tela com base na direção e no quadro atual
void DesenharFantasma(Ghost *g) {
    Texture2D *animation[] = { g->up, g->down, g->left, g->right };

    int indice = 0;
    if (g->frameCount[g->currentDirection] > 1) {
        indice = g->frameIndex;
    }

    DrawTexture(animation[g->currentDirection][indice], g->posicao.x, g->posicao.y, WHITE);
}

// Libera as texturas usadas pelo fantasma da memória
void DestruirFantasma(Ghost *g) {
    for (int d = 0; d < 4; d++) { // Para cada direção
        Texture2D *sprites[] = { g->up, g->down, g->left, g->right };
        for (int i = 0; i < g->frameCount[d]; i++) {
            UnloadTexture(sprites[d][i]); // Libera cada quadro de animação
        }
    }
}


bool VerificarColisaoFantasma(Ghost *g, Jogador *j, Sound somSusto) {
    // Cria retângulos representando o fantasma e o jogador
    Rectangle retJogador = { j->posicao.x, j->posicao.y, 32, 32 };
    Rectangle retFantasma = { g->posicao.x, g->posicao.y, g->right[0].width, g->down[0].height };

    // Verifica colisão entre jogador e fantasma
    if (CheckCollisionRecs(retJogador, retFantasma)) {
        PlaySound(somSusto); // Toca som de susto
        j->posicao = (Vector2){ WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f }; // Reinicia posição do jogador
        return true;
    }
    return false;
}

void InicializarListaFantasmas(ListaFantasmas *lista, const char *caminhoJSON, const char *chaveFantasma) {
    lista->quantidade = 1;
    lista->tempoDesdeUltimo = 0.0f;
    lista->fantasmas[0] = CriarFantasma(caminhoJSON, chaveFantasma, (Vector2){ 300, 300 });
}


void AtualizarListaFantasmas(ListaFantasmas *lista, Vector2 jogadorPos, Rectangle areaJogo, float delta) {
    lista->tempoDesdeUltimo += delta;

    // A cada 30 segundos, adiciona um novo fantasma (até o máximo)
    if (lista->tempoDesdeUltimo >= 30.0f && lista->quantidade < MAX_FANTASMAS) {
        lista->fantasmas[lista->quantidade] = CriarFantasma(
            "sprites/json/movimentaçãoPlayer.json", "ghost_walk",
            (Vector2){
                rand() % (int)(areaJogo.width - 64) + (int)areaJogo.x,
                rand() % (int)(areaJogo.height - 64) + (int)areaJogo.y
            }
        );

        lista->quantidade++;
        lista->tempoDesdeUltimo = 0.0f;
    }

    // Atualiza todos os fantasmas
    for (int i = 0; i < lista->quantidade; i++) {
        AtualizarFantasma(&lista->fantasmas[i], jogadorPos, areaJogo);
    }
}


void DesenharListaFantasmas(ListaFantasmas *lista) {
    for (int i = 0; i < lista->quantidade; i++) {
        DesenharFantasma(&lista->fantasmas[i]);
    }
}

void DestruirListaFantasmas(ListaFantasmas *lista) {
    for (int i = 0; i < lista->quantidade; i++) {
        DestruirFantasma(&lista->fantasmas[i]);
    }
}

Ghost2 CriarGhost2(const char *caminhoJSON, const char *chaveFantasma, Vector2 posicao) {
    Ghost2 g2;
    g2.eixoAtual = MOVENDO_X;
    g2.tempoEixo = 0.2f; // Tempo para mudar eixo de movimento

    g2.ghost = CriarFantasma(caminhoJSON, chaveFantasma, posicao);
    g2.velocidade = g2.ghost.velocidade;
    g2.tempoTextoMoeda = 0.0f;
    return g2;
}


Vector2 Normaliza(Vector2 v) {
    float comprimento = sqrtf(v.x * v.x + v.y * v.y);
    if (comprimento == 0) return (Vector2){ 0, 0 };
    return (Vector2){ v.x / comprimento, v.y / comprimento };
}


void VerificarColisaoGhost2(Ghost2 *ghost, Jogador *j, Sound somColisao, TextoPerda textos[MAX_TEXTS]) {
    float tempoAtual = GetTime();

    // Retângulo do jogador
    Rectangle rectJogador = {
        j->posicao.x,
        j->posicao.y,
        j->direita[0].width,
        j->direita[0].height
    };

    // Pega a textura do fantasma na direção e quadro atuais
    Texture2D *directions[] = {
        ghost->ghost.up,
        ghost->ghost.down,
        ghost->ghost.left,
        ghost->ghost.right
    };

    Texture2D texturaAtual = directions[ghost->ghost.currentDirection][ghost->ghost.frameIndex];

    Rectangle rectGhost = {
        ghost->ghost.posicao.x,
        ghost->ghost.posicao.y,
        texturaAtual.width,
        texturaAtual.height
    };

    // Colisão detectada
    if (CheckCollisionRecs(rectJogador, rectGhost)) {
        if (tempoAtual - j->tempoUltimaColisaoGhost2 >= 10.0f) { // Delay de 10s entre colisões
            PlaySound(somColisao);
            j->tempoUltimaColisaoGhost2 = tempoAtual;

            // Perde 1 moeda, priorizando ouro
            if (j->moedasOuro > 0) j->moedasOuro--;
            else if (j->moedasPrata > 0) j->moedasPrata--;

            // Ativa um texto flutuante "-1 moeda"
            for (int i = 0; i < MAX_TEXTS; i++) {
                if (!textos[i].active) {
                    textos[i].active = true;
                    textos[i].posicao = (Vector2){ 
                        j->posicao.x + j->direita[0].width / 2 - MeasureText("-1 moeda", 20) / 2, 
                        j->posicao.y - 10 
                    };
                    textos[i].Congelado = tempoAtual + 5.0f;
                    strcpy(textos[i].texto, "-1 moeda");
                    break;
                }
            }

            j->tempoTextoMoeda = tempoAtual + 5.0f;

            // Recuo do fantasma após colisão
            Vector2 des = {
                ghost->ghost.posicao.x - j->posicao.x,
                ghost->ghost.posicao.y - j->posicao.y
            };

            Vector2 direcaoOposta = Normaliza(des);

            // Empurra o fantasma para longe do jogador
            ghost->ghost.posicao.x += direcaoOposta.x * 1200.0f;
            ghost->ghost.posicao.y += direcaoOposta.y * 1200.0f;

            // Corrige se o fantasma sair da área do jogo
            if (ghost->ghost.posicao.x < areaJogo.x)
                ghost->ghost.posicao.x = areaJogo.x;
            if (ghost->ghost.posicao.x + ghost->ghost.right[0].width > areaJogo.x + areaJogo.width)
                ghost->ghost.posicao.x = areaJogo.x + areaJogo.width - ghost->ghost.right[0].width;

            if (ghost->ghost.posicao.y < areaJogo.y)
                ghost->ghost.posicao.y = areaJogo.y;
            if (ghost->ghost.posicao.y + ghost->ghost.down[0].height > areaJogo.y + areaJogo.height)
                ghost->ghost.posicao.y = areaJogo.y + areaJogo.height - ghost->ghost.down[0].height;
        }
    }
}

void DesenharTextosPerda(TextoPerda textos[MAX_TEXTS]) {
    // Percorre todos os textos possíveis na lista
    for (int i = 0; i < MAX_TEXTS; i++) {

        // Verifica se esse texto está ativo (ou seja, deve ser desenhado na tela)
        if (textos[i].active) {

            // Desenha o texto na posição definida, um pouco acima (y - 20) e na cor vermelha
            DrawText(textos[i].texto,
                     textos[i].posicao.x,
                     textos[i].posicao.y - 20,  // sobe um pouco o texto em relação ao personagem
                     20,                        // tamanho da fonte
                     RED);                      // cor do texto

            // Verifica se o tempo atual já passou do tempo de congelamento do texto
            if (GetTime() > textos[i].Congelado) {
                // Desativa o texto, ele não será mais desenhado nas próximas atualizações
                textos[i].active = false;
            }
        }
    }
}



void AtualizarGhost2(Ghost2 *ghost, Vector2 jogadorPos, Rectangle areaJogo) {
    // Obtém o tempo entre os frames, para manter a movimentação suave e com base no tempo real
    float dt = GetFrameTime();

    // Atualiza o temporizador da animação do fantasma
    ghost->ghost.timer += dt;

    // Diminui o tempo restante até poder trocar o eixo de movimentação (X <-> Y)
    ghost->tempoEixo -= dt;

    // Calcula a distância horizontal (dx) e vertical (dy) entre o fantasma e o jogador
    float dx = jogadorPos.x - ghost->ghost.posicao.x;
    float dy = jogadorPos.y - ghost->ghost.posicao.y;

    // Define a velocidade reduzida do Ghost2 (40% da original, para ser mais lento que outros)
    float velocidade = ghost->velocidade * 0.4f;

    // Verifica se é hora de mudar o eixo de movimentação
    if (ghost->tempoEixo <= 0) {
        // Se estiver movendo em X e a distância em Y for maior, troca para o eixo Y
        if (ghost->eixoAtual == MOVENDO_X && fabs(dy) > fabs(dx)) {
            ghost->eixoAtual = MOVENDO_Y;
            ghost->tempoEixo = 0.2f;  // reinicia o tempo para próxima troca de eixo
        }
        // Se estiver movendo em Y e a distância em X for maior, troca para o eixo X
        else if (ghost->eixoAtual == MOVENDO_Y && fabs(dx) > fabs(dy)) {
            ghost->eixoAtual = MOVENDO_X;
            ghost->tempoEixo = 0.2f;
        }
    }

    // Movimento no eixo X (horizontal)
    if (ghost->eixoAtual == MOVENDO_X) {
        if (dx > 0 && ghost->ghost.posicao.x + ghost->ghost.right[0].width < areaJogo.x + areaJogo.width) {
            // Move para a direita se ainda estiver dentro da área de jogo
            ghost->ghost.posicao.x += velocidade;
            ghost->ghost.currentDirection = RIGHT;
        } else if (dx < 0 && ghost->ghost.posicao.x > areaJogo.x) {
            // Move para a esquerda se ainda estiver dentro da área de jogo
            ghost->ghost.posicao.x -= velocidade;
            ghost->ghost.currentDirection = LEFT;
        }
    }
    // Movimento no eixo Y (vertical)
    else {
        if (dy > 0 && ghost->ghost.posicao.y + ghost->ghost.down[0].height < areaJogo.y + areaJogo.height) {
            // Move para baixo se ainda estiver dentro da área de jogo
            ghost->ghost.posicao.y += velocidade;
            ghost->ghost.currentDirection = DOWN;
        } else if (dy < 0 && ghost->ghost.posicao.y > areaJogo.y) {
            // Move para cima se ainda estiver dentro da área de jogo
            ghost->ghost.posicao.y -= velocidade;
            ghost->ghost.currentDirection = UP;
        }
    }

    // Atualiza a animação do fantasma com base no tempo acumulado
    int totalFrames = ghost->ghost.frameCount[ghost->ghost.currentDirection];
    if (totalFrames > 1 && ghost->ghost.timer >= ghost->ghost.frameTime) {
        // Avança para o próximo frame da animação
        ghost->ghost.frameIndex = (ghost->ghost.frameIndex + 1) % totalFrames;
        ghost->ghost.timer = 0; // reinicia o temporizador da animação
    }
}


