#include "menu.h"  // Inclui o cabeçalho do módulo menu, onde estão as declarações e tipos usados

#define TOTAL_OPCOES_MENU 3  // Define o total de opções no menu principal

// Função para atualizar a seleção do menu com base na entrada do teclado
// Recebe um botão (pode ser usado para detecção de clique futuramente), um ponteiro para a tela atual e para a opção selecionada
void AtualizarMenu(Rectangle botao, TelaAtual *telaAtual, int *opcao) {
    // Se a tecla para baixo for pressionada, avança a seleção, ciclando de volta para o começo
    if (IsKeyPressed(KEY_DOWN)) {
        *opcao = (*opcao + 1) % TOTAL_OPCOES_MENU;
    }
    // Se a tecla para cima for pressionada, volta a seleção, ciclando para o fim se for necessário
    if (IsKeyPressed(KEY_UP)) {
        *opcao = (*opcao - 1 + TOTAL_OPCOES_MENU) % TOTAL_OPCOES_MENU;
    }

    // Se a tecla ENTER ou ESPAÇO for pressionada, seleciona a opção atual e muda a tela conforme
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        switch (*opcao) {
            case 0:
                *telaAtual = SELECAO;  // Vai para a tela de seleção de jogadores
                break;
            case 1:
                *telaAtual = SOBRE;    // Vai para a tela "Sobre"
                break;
            case 2:
                *telaAtual = RANKING;  // Vai para a tela de ranking
                break;
        }
    }
}

#define TAM_OPCAO 20         // Define o tamanho máximo da string para cada opção do menu
#define TOTAL_OPCOES_MENU 3  // Já definido anteriormente, aqui apenas repetido para clareza

// Array de strings contendo as opções do menu principal
char opcoes[TOTAL_OPCOES_MENU][TAM_OPCAO] = {
    "INICIAR",  // Iniciar o jogo
    "SOBRE",    // Informações sobre o jogo
    "RANKING"   // Mostrar ranking de pontuações
};

// Função para desenhar o menu principal na tela
// Recebe o retângulo do botão (não usado diretamente aqui, mas pode ser para interação futura), a textura de fundo e a opção selecionada
void DesenharMenu(Rectangle botao, Texture2D background, int opcao) {
    ClearBackground(RAYWHITE);  // Limpa a tela com a cor branca padrão do Raylib

    // Desenha o background da tela escalado para cobrir toda a janela
    DrawTexturePro(
        background,
        (Rectangle){ 0, 0, background.width, background.height },  // Área da textura original
        (Rectangle){ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() }, // Área destino na tela
        (Vector2){ 0, 0 },  // Origem do desenho
        0.0f,               // Rotação
        WHITE               // Cor (branco = sem alteração)
    );

    // Configurações do título
    int fonteTitulo = 40;
    const char *titulo = "CoinHunting!";
    int larguraTitulo = MeasureText(titulo, fonteTitulo);

    // Desenha o título centralizado horizontalmente e com cor cinza escuro
    DrawText(titulo, (GetScreenWidth() - larguraTitulo) / 2, 100, fonteTitulo, DARKGRAY);

    // Configurações para os botões de menu
    int largura = 200;
    int altura = 50;
    int x = GetScreenWidth() / 2 - largura / 2;  // Centraliza horizontalmente
    int y = 200;  // Posição vertical inicial
    Color corNormal = LIGHTGRAY;  // Cor dos botões não selecionados
    Color corSelecionada = YELLOW; // Cor do botão selecionado

    // Loop para desenhar cada opção do menu
    for (int i = 0; i < TOTAL_OPCOES_MENU; i++) {
        // Define a cor conforme se a opção está selecionada ou não
        Color cor = (i == opcao) ? corSelecionada : corNormal;

        // Desenha o retângulo do botão
        DrawRectangle(x, y + i * 70, largura, altura, cor);

        // Calcula a posição do texto para centralizá-lo dentro do botão
        int textoX = x + (largura - MeasureText(opcoes[i], 20)) / 2;

        // Desenha o texto da opção em preto, posicionado dentro do botão
        DrawText(opcoes[i], textoX, y + i * 70 + 15, 20, BLACK);
    }
}
