// Evita que esse cabeçalho seja incluído mais de uma vez no mesmo arquivo.
#ifndef RANKING_H
#define RANKING_H

// Define o tamanho máximo do nome (5 caracteres + '\0' para o fim da string)
#define TAM_NOME 6 

// Define a estrutura que representa um nó da lista de ranking
typedef struct Ranking {
    char nome[TAM_NOME];        // Nome do jogador (string com tamanho fixo)
    int pontuacao;              // Pontuação do jogador
    struct Ranking *next;       // Ponteiro para o próximo elemento da lista
} Ranking;

// Função que carrega o ranking de um arquivo e retorna um ponteiro para a cabeça da lista
Ranking *CarregarRanking();

// Função que salva a lista de ranking em um arquivo
void SalvarRanking(Ranking **head);

// Função que adiciona um novo jogador com pontuação ao ranking (lista ligada)
void AdicionarAoRanking(Ranking **head, const char *nome, int pontuacao);

// Função que libera a memória alocada para a lista de ranking
void LiberarRanking(Ranking **head);

// Função que exibe/desenha o ranking (por exemplo, no terminal ou interface gráfica)
void DesenharRanking(Ranking **head);

// Fim da diretiva de inclusão condicional
#endif
