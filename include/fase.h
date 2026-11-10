#ifndef FASE_H
#define FASE_H

#include <stdbool.h>
#include "raylib.h"
#include "plataformas.h"

#define MAX_PLATAFORMAS 64

typedef struct Fase {
    int id;
    Plataforma plataformas[MAX_PLATAFORMAS];
    int qtdPlataformas;
    float largura;
    Vector2 pontoInicial;
    Rectangle areaSaida;
    float limiteQueda;
} Fase;

bool CarregarFase(Fase *fase, int id);
void DesenharFase(const Fase *fase);

#endif