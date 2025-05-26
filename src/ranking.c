#include "ranking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"

// Função que carrega os dados do ranking a partir de um arquivo de texto
Ranking *CarregarRanking() {
    FILE *arquivo = fopen("arquivo/ranking.txt", "r");  // Abre o arquivo em modo leitura
    if (!arquivo){
        return NULL;  // Se o arquivo não existir, retorna NULL
    } 

    Ranking *inicio = NULL;   // Ponteiro para o início da lista encadeada
    Ranking *ultimo = NULL;   // Ponteiro para o último nó da lista

    char nome[TAM_NOME];
    int pontos;

    // Lê os dados do arquivo até o final
    while (fscanf(arquivo, "%5s %d", nome, &pontos) == 2) {
        struct Ranking *novo_no = (struct Ranking*)malloc(sizeof(struct Ranking)); // Cria novo nó
        strcpy(novo_no->nome, nome);
        novo_no->pontuacao = pontos;
        novo_no->next = NULL;

        if (!inicio){
            inicio = novo_no;  // Primeiro elemento da lista
        } else {
            ultimo->next = novo_no;  // Liga o novo elemento ao final da lista
        }

        ultimo = novo_no;  // Atualiza o ponteiro para o último
    }

    fclose(arquivo);  // Fecha o arquivo
    return inicio;  // Retorna o início da lista
}

// Salva os dados da lista de ranking no arquivo
void SalvarRanking(Ranking **head) {
    FILE *arquivo = fopen("arquivo/ranking.txt", "w");  // Abre arquivo em modo escrita
    if (arquivo == NULL){
        return;
    } 
    Ranking *temp = *head;
    while (temp != NULL) {
        fprintf(arquivo, "%s %d\n", temp->nome, temp->pontuacao);  // Escreve nome e pontuação
        temp = temp->next;
    }

    fclose(arquivo);  // Fecha o arquivo
}

// Adiciona um novo jogador ao ranking, mantendo a lista ordenada por pontuação
void AdicionarAoRanking(Ranking **head, const char *nome, int pontuacao) {
    struct Ranking *novo_no =(struct Ranking*)malloc(sizeof(struct Ranking));
    strncpy(novo_no->nome, nome, TAM_NOME - 1);
    novo_no->nome[TAM_NOME-1] = '\0';  // Garante que a string tenha fim
    novo_no->pontuacao = pontuacao;
    novo_no->next = NULL;

    // Insere no início se a lista estiver vazia ou se a nova pontuação for maior que a primeira
    if (*head == NULL || pontuacao > (*head)->pontuacao) {
        novo_no->next = *head;
        *head = novo_no;
    } else {
        Ranking *temp = *head;
        while (temp->next && temp->next->pontuacao >= pontuacao) {
            temp = temp->next;
        }
        novo_no->next = temp->next;
        temp->next = novo_no;
    }
}

// Libera a memória usada pela lista do ranking
void LiberarRanking(Ranking **head) {
    while (*head != NULL) {
        struct Ranking *temp = *head;
        *head = (*head)->next;
        free(temp);  // Libera cada nó da lista
    }
}

// Desenha a tela do ranking
void DesenharRanking(Ranking **head) {
    static int scrollOffset = 0;  // Controle de rolagem para cima e para baixo
    int totalExibidos = 5;  // Número de jogadores exibidos por vez

    ClearBackground(DARKGRAY);  // Cor de fundo da tela

    // Título do ranking
    DrawText("TOP JOGADORES", 
            GetScreenWidth()/2 - MeasureText("TOP JOGADORES", 30)/2, 
            20, 
            30, 
            GOLD);

    DrawText("Melhores Pontuações", 
            GetScreenWidth()/2 - MeasureText("Melhores Pontuações", 20)/2, 
            60, 
            20, 
            LIGHTGRAY);

    // Conta quantos jogadores existem na lista
    int totalJogadores = 0;
    Ranking *conta = *head;
    while (conta != NULL) {
        totalJogadores++;
        conta = conta->next;
    }

    // Controla rolagem para baixo
    if (IsKeyPressed(KEY_DOWN) && scrollOffset + totalExibidos < totalJogadores) {
        scrollOffset++;
    }
    // Controla rolagem para cima
    if (IsKeyPressed(KEY_UP) && scrollOffset > 0) {
        scrollOffset--;
    }

    // Pula os primeiros elementos de acordo com a rolagem
    Ranking *temp = *head;
    int i = 0;
    while (i < scrollOffset && temp != NULL) {
        temp = temp->next;
        i++;
    }

    int y = 100;  // Posição vertical inicial
    int pos = scrollOffset + 1;  // Posição de rank
    int exibidos = 0;  // Contador de quantos foram desenhados

    // Desenha cada jogador da lista (até 5 por vez)
    while (temp && exibidos < totalExibidos) {
        char texto[64];
        snprintf(texto, sizeof(texto), "%d. %s - %d", pos, temp->nome, temp->pontuacao);  // Texto do jogador

        Color corTexto = WHITE;
        Color fundo = Fade(LIGHTGRAY, 0.2f);  // Fundo com transparência

        if (pos == 1) {
            fundo = GOLD;        // Destaque para o primeiro lugar
            corTexto = BLACK;
        } else if (pos % 2 == 0) {
            corTexto = SKYBLUE;  // Destaque para posições pares
        }

        int boxWidth = 400;
        int boxHeight = 35;
        int x = GetScreenWidth()/2 - boxWidth/2;  // Centraliza horizontalmente

        DrawRectangleRounded((Rectangle){x, y, boxWidth, boxHeight}, 0.3f, 10, fundo);  // Caixa do jogador
        DrawText(texto, x + 10, y + 5, 25, corTexto);  // Texto do jogador

        y += 45;
        pos++;
        exibidos++;
        temp = temp->next;  // Vai para o próximo jogador da lista
    }
}