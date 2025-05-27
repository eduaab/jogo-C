#include "fixo.h"

// Define a área do jogo na tela (posição x,y e tamanho largura, altura)
Rectangle areaJogo = { 130, 40, 1030, 600};

/**
 * Carrega texturas a partir de um array JSON de strings com caminhos.
 * 
 * @param imagens - vetor já alocado para armazenar as texturas carregadas.
 * @param array - objeto cJSON que deve ser um array de strings (caminhos).
 * @param quantidade - ponteiro para inteiro que receberá a quantidade de texturas carregadas.
 */
void CarregarTexturas(Texture2D *imagens, cJSON *array, int *quantidade) {
    int total = cJSON_GetArraySize(array); // Obtém o tamanho do array JSON
    *quantidade = total;                    // Guarda o total

    for (int i = 0; i < total; i++) {
        cJSON *item_imagem = cJSON_GetArrayItem(array, i); // Pega o i-ésimo item do array
        if (cJSON_IsString(item_imagem) && (item_imagem->valuestring != NULL)) {
            char *caminho = item_imagem->valuestring;     // Obtém o caminho do arquivo
            imagens[i] = LoadTexture(caminho);             // Carrega a textura e armazena no vetor
        } else {
            // Caso o item não seja uma string válida, carrega uma textura padrão (ou trata o erro)
            // Por exemplo, carregue uma textura vazia ou informe um erro:
            // imagens[i] = LoadTexture("textura_default.png");
            // Ou apenas continue sem carregar
            imagens[i] = (Texture2D){0}; // textura inválida, para evitar lixo na memória
        }
    }
}
