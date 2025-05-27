#include "fixo.h"         // Cabeçalho com definições fixas (como limites, cores, etc.)
#include "ghost.h"        // Cabeçalho que define estruturas e funções dos fantasmas
#include "cJSON.h"        // Biblioteca para ler arquivos JSON
#include <stdio.h>
#include <stdlib.h>

// Função para criar e inicializar o jogador a partir de um JSON
Jogador CriarJogador(const char *jsonPath, const char *nome, Vector2 pos0)
{
    Jogador j = {0}; // Zera todos os valores do jogador
    j.posicao      = pos0;
    j.velocidade   = 4;
    j.direcaoAtual = DOWN;
    j.tempoFrame   = 0.15f;
    j.tempoUltimaColisaoGhost2 = -10.0f; // Inicializa tempo para evitar bug de colisão imediata

    // Abre o arquivo JSON com os dados do jogador
    FILE *f = fopen(jsonPath, "rb");
    if (f == NULL){
        return j; // Se não conseguir abrir, retorna jogador com valores padrão
    } 

    // Move ponteiro pro final pra saber o tamanho do arquivo
    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    rewind(f); // Volta ao início

    // Aloca espaço para o conteúdo do arquivo
    char *buffer = malloc(tamanho + 1);
    if (buffer == NULL) {
        fclose(f);
        return j;
    }

    // Lê todo o arquivo para a memória
    size_t lidos = fread(buffer, 1, tamanho, f);
    fclose(f);

    if (lidos != tamanho) {
        free(buffer);
        return j;
    }

    buffer[tamanho] = '\0';  // Termina string

    // Faz o parse do JSON
    cJSON *root = cJSON_Parse(buffer);
    free(buffer);
    if (root == NULL){
        return j;
    } 

    // Pega o objeto específico do jogador no JSON (por chave)
    cJSON *obj = cJSON_GetObjectItem(root, nome);
    if (obj == NULL) {
        cJSON_Delete(root);
        return j;
    }

    // Carrega as texturas de cada direção (sprites)
    int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
    CarregarTexturas(j.cima,     cJSON_GetObjectItem(obj,"up"),    &q1);
    CarregarTexturas(j.baixo,    cJSON_GetObjectItem(obj,"down"),  &q2);
    CarregarTexturas(j.esquerda, cJSON_GetObjectItem(obj,"left"),  &q3);
    CarregarTexturas(j.direita,  cJSON_GetObjectItem(obj,"right"), &q4);

    cJSON_Delete(root);
    return j;
}

// Atualiza o índice da animação do jogador com base no tempo
void AtualizarAnimacao(Jogador *j, int andando)
{
    if (!andando){
        j->indiceFrame = 0; // Parado: volta pro primeiro frame
        return;
    }  

    // Define quantos frames usar com base na direção
    int maxFrames;
    if (j->direcaoAtual == LEFT || j->direcaoAtual == RIGHT){
        maxFrames = MAX_FRAMES_LADO;
    } else {
        maxFrames = MAX_FRAMES_CIMA_BAIXO;
    }             

    j->temporizador += GetFrameTime(); // Tempo desde último frame
    if (j->temporizador >= j->tempoFrame) {
        j->indiceFrame = (j->indiceFrame + 1) % maxFrames; // Avança pro próximo frame
        j->temporizador = 0.0f;
    }
}

// Atualiza a posição e direção do jogador com base no teclado
void AtualizarJogador(Jogador *j, int upKey, int downKey, int leftKey, int rightKey, Rectangle areaJogo)
{
    // Se ainda está no tempo de "invencibilidade", não move
    if (GetTime() - j->tempoUltimaColisaoGhost2 < 5.0f) {
        return;
    }

    int andando = 0; // Flag para verificar se o jogador está andando
    
    if (IsKeyDown(upKey) && j->posicao.y > areaJogo.y) {
        j->posicao.y -= j->velocidade;
        j->direcaoAtual = UP;
        andando = 1;
    } else if (IsKeyDown(downKey) && j->posicao.y + j->baixo[0].height < areaJogo.y + areaJogo.height) {
        j->posicao.y += j->velocidade;
        j->direcaoAtual = DOWN;
        andando = 1;
    } else if (IsKeyDown(leftKey) && j->posicao.x > areaJogo.x) {
        j->posicao.x -= j->velocidade;
        j->direcaoAtual = LEFT;
        andando = 1;
    } else if (IsKeyDown(rightKey) && j->posicao.x + j->direita[0].width < areaJogo.x + areaJogo.width) {
        j->posicao.x += j->velocidade;
        j->direcaoAtual = RIGHT;
        andando = 1;
    }

    AtualizarAnimacao(j, andando); // Atualiza animação de acordo com o movimento
}

// Desenha o jogador na tela
void DesenharJogador(Jogador *j){
    Texture2D txt;

    // Escolhe o sprite com base na direção
    switch (j->direcaoAtual) {
        case UP:    txt = j->cima    [j->indiceFrame]; break;
        case DOWN:  txt = j->baixo   [j->indiceFrame]; break;
        case LEFT:  txt = j->esquerda[j->indiceFrame]; break;
        case RIGHT: txt = j->direita [j->indiceFrame]; break;
    }

    Color cor = WHITE;

    // Se colidiu com Ghost2 recentemente, pisca em vermelho
    if (GetTime() - j->tempoUltimaColisaoGhost2 < 0.5f) {
        cor = RED;
    }

    DrawTexture(txt, j->posicao.x, j->posicao.y, cor);
}

// Libera as texturas do jogador da memória
void DestruirJogador(Jogador *j)
{
    for (int i = 0; i < MAX_FRAMES_CIMA_BAIXO; i++) {
        UnloadTexture(j->cima[i]);
        UnloadTexture(j->baixo[i]);
        if (i < MAX_FRAMES_LADO) {
            UnloadTexture(j->esquerda[i]);
            UnloadTexture(j->direita[i]);
        }
    }
}
