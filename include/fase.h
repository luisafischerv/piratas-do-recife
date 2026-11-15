#ifndef FASE_H
#define FASE_H

#include <stdbool.h>
#include "raylib.h"
#include "plataformas.h"
#include "jogador.h"

#define MAX_PLATAFORMAS 64

typedef struct Fase {
    int id;
    Plataforma plataformas[MAX_PLATAFORMAS];
    int qtdPlataformas;
    float largura;
    Vector2 pontoInicial;
    Rectangle areaSaida;
    float limiteQueda;
    Texture2D fundo;
    Texture2D fundo2;
    Texture2D chao;
    Texture2D caixa;
    Texture2D passarela;
    Texture2D cargas;
    Texture2D raizes;
    Texture2D plataformaFragil;
} Fase;

bool CarregarFase(Fase *fase, int id);
void DesenharFase(const Fase *fase);
void DescarregarFase(Fase *fase);
void AtualizarPlataformasFrageis(Fase *fase, const Jogador *jogador, float dt);

#endif
