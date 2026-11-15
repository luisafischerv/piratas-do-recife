#ifndef RANKING_H
#define RANKING_H

#include <stdbool.h>

#define MAX_RANKING 10
#define TAM_APELIDO 13              // 12 letras + '\0'
#define ARQUIVO_RANKING "ranking.txt"

typedef struct Resultado {
    char apelido[TAM_APELIDO];
    double tempo;                   // segundos da campanha (menor e melhor)
    int ordemRegistro;              // 0 = mais antigo; desempata tempos iguais
} Resultado;

typedef struct Ranking {
    Resultado resultados[MAX_RANKING];
    int quantidade;
    int proximaOrdem;               // ordem que o proximo resultado vai receber
} Ranking;

void InicializarRanking(Ranking *ranking);

// Insertion Sort por tempo crescente. Empate: o mais antigo fica antes.
void OrdenarRanking(Resultado v[], int n);

// Registra uma vitoria. Retorna true se ela entrou entre os 10 melhores.
// Chamar UMA vez por campanha concluida.
bool RegistrarResultado(Ranking *ranking, const char *apelido, double tempo);

// Le o arquivo. Se ele nao existir ou estiver estragado, o ranking fica
// vazio ou so com as linhas validas; nunca derruba o jogo.
// Retorna quantos resultados foram carregados.
int CarregarRanking(Ranking *ranking, const char *arquivo);

// Grava uma linha por resultado: apelido;tempo;ordemRegistro
// Retorna false se nao conseguiu gravar (o jogo continua normalmente).
bool SalvarRanking(const Ranking *ranking, const char *arquivo);

// Escreve o tempo como "mm:ss.mmm" (ex.: 583.25 -> "09:43.250").
void FormatarTempo(double tempo, char *saida, int tamanho);

void DesenharRanking(const Ranking *ranking, int x, int y);

#endif
