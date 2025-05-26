#include "selecao.h"   // Cabeçalho do módulo de seleção, deve conter definição de TelaAtual, etc.
#include "raylib.h"    // Biblioteca gráfica usada para desenho e entrada de teclado

#define TOTAL_OPCOES_SELECAO 2  // Define o número total de opções no menu de seleção

// Atualiza a seleção de jogadores com base na entrada do teclado
void AtualizarSelecaoJogadores(Rectangle botao1, Rectangle botao2, TelaAtual *tela, int *opcao)
{
    // Move a seleção para baixo
    if (IsKeyPressed(KEY_DOWN)) {
        *opcao = (*opcao + 1) % TOTAL_OPCOES_SELECAO;
    }

    // Move a seleção para cima
    if (IsKeyPressed(KEY_UP)) {
        *opcao = (*opcao - 1 + TOTAL_OPCOES_SELECAO) % TOTAL_OPCOES_SELECAO;
    }

    // Confirma a seleção com ENTER ou BARRA DE ESPAÇO
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        *tela = JOGO;  // Muda para a tela do jogo

        // Define o número de jogadores baseado na opção selecionada
        if (*opcao == 0){
            *opcao = 1;  // 1 jogador
        }  
        else if (*opcao == 1){
            *opcao = 2;  // 2 jogadores
        } else {
            *tela = MENU;  // Segurança: volta para o menu em caso de erro inesperado
        }   
    }
}

// Desenha a interface da tela de seleção de jogadores
void DesenharSelecaoJogadores(Rectangle botao1, Rectangle botao2, Texture2D background, int opcao)
{
    // Desenha o fundo da tela (preenchendo a tela inteira com o background)
    DrawTexturePro(
        background,
        (Rectangle){ 0, 0, background.width, background.height }, // Área da textura original
        (Rectangle){ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() }, // Área destino
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );

    // Título da tela
    const char *titulo = "Selecione o número de jogadores:";
    int fonteTitulo = 30;
    int larguraTitulo = MeasureText(titulo, fonteTitulo);

    // Centraliza e desenha o título
    DrawText(titulo, (GetScreenWidth() - larguraTitulo) / 2, 100, fonteTitulo, WHITE);

    // Define posição e tamanho dos botões
    int largura = 220;
    int altura = 50;
    int x = GetScreenWidth() / 2 - largura / 2;  // Centraliza horizontalmente
    int y1 = 200;  // Posição vertical do botão 1 jogador
    int y2 = 270;  // Posição vertical do botão 2 jogadores

    // Cores para botão selecionado e não selecionado
    Color corSelecionada = YELLOW;
    Color corNormal = LIGHTGRAY;
    Color cor;

    // Desenha o botão "1 JOGADOR"
    cor = (opcao == 0) ? corSelecionada : corNormal;
    DrawRectangle(x, y1, largura, altura, cor);
    DrawText("1 JOGADOR", x + 40, y1 + 15, 20, BLACK);

    // Desenha o botão "2 JOGADORES"
    cor = (opcao == 1) ? corSelecionada : corNormal;
    DrawRectangle(x, y2, largura, altura, cor);
    DrawText("2 JOGADORES", x + 30, y2 + 15, 20, BLACK);
}

// Verifica se o jogador quer sair da tela "Sobre o Jogo"
void AtualizarSobre(TelaAtual *tela) {
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
        *tela = MENU;  // Volta para o menu principal
    }
}

// Desenha a tela "Sobre o Jogo"
void DesenharSobre(Texture2D background)
{
    // Desenha o fundo da tela
    DrawTexturePro(
        background,
        (Rectangle){ 0, 0, background.width, background.height },
        (Rectangle){ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );

    // Título e descrições do jogo
    const char *titulo = "Sobre o Jogo";
    const char *descricao = "Ajude os exploradores a coletarem o máximo de moedas possivel\n sem ser atingido pelos fantasmas protetores da caverna.\n";

    const char *descricao2 =
        "Fantasma vermelho: Tira moedas e congela o jogador por um tempo, apesar de ser lerdo, ele lhe seguirá pelo mapa.\n\n"
        "Fantasma azul: Joga o jogador pro centro do mapa, são rápidos, mas andam aleatoriamente pelo mapa.\n";

    const char *descricao3 =
        "\nControles:\n"
        "Jogador 1: W A S D \n"
        "Jogador 2: Setas";

    // Tamanhos de fonte e larguras de texto para centralização
    int fonteTitulo = 30;
    int fonteDescricao = 20;
    int larguraTitulo = MeasureText(titulo, fonteTitulo);
    int larguraDescricao = MeasureText(descricao, fonteDescricao);
    int larguraDescricao2 = MeasureText(descricao2, fonteDescricao);
    int larguraDescricao3 = MeasureText(descricao3, fonteDescricao);

    // Desenha os textos, centralizando-os horizontalmente
    DrawText(titulo, (GetScreenWidth() - larguraTitulo) / 2, 100, fonteTitulo, WHITE);
    DrawText(descricao, (GetScreenWidth() - larguraDescricao) / 2, 180, fonteDescricao, WHITE);
    DrawText(descricao2, (GetScreenWidth() - larguraDescricao2) / 2, 260, fonteDescricao, WHITE);

    // Corrigido para centralizar texto corretamente
    DrawText(descricao3, (GetScreenWidth() - larguraDescricao3) / 2, 320, fonteDescricao, WHITE);
}
