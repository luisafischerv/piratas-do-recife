#ifndef VITORIA_H
#define VITORIA_H

#include <stdbool.h>
#include "ranking.h"

typedef enum EtapaVitoria {
    VITORIA_FINAL,
    VITORIA_APELIDO,
    VITORIA_RANKING
} EtapaVitoria;

typedef struct TelaVitoria {
    EtapaVitoria etapa;
    double tempoFinal;
    char apelido[TAM_APELIDO];
    int tamanhoApelido;
    bool registrado;
    bool entrouNoRanking;
    int posicao;
} TelaVitoria;

void IniciarVitoria(TelaVitoria *v, double tempoFinal);
bool ConfirmarApelido(TelaVitoria *v, Ranking *ranking, const char *arquivo);
bool AtualizarVitoria(TelaVitoria *v, Ranking *ranking);
void DesenharVitoria(const TelaVitoria *v, const Ranking *ranking);

#endif
