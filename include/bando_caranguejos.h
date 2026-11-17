#ifndef BANDO_CARANGUEJOS_H
#define BANDO_CARANGUEJOS_H
#include "jogador.h"

#define CARANGUEJOS_POR_BANDO 6
typedef struct BandoCaranguejos {
    Rectangle corpos[CARANGUEJOS_POR_BANDO];
    bool ativo;
    bool atingiu[CARANGUEJOS_POR_BANDO];
    int direcao;
    int quantidade;
    float espera;
} BandoCaranguejos;

void ReiniciarBando(BandoCaranguejos *b);
int AtualizarBando(BandoCaranguejos *b, const Jogador *j, int estagio, float dt);
void DesenharBando(const BandoCaranguejos *b);
#endif
