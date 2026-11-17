#ifndef CIPO_H
#define CIPO_H
#include "jogador.h"

#define QUANTIDADE_CIPOS 4
typedef struct Cipo {
    Vector2 pivo;
    float comprimento;
    float angulo;
    float velocidadeAngular;
} Cipo;
typedef struct SistemaCipos {
    Cipo cipos[QUANTIDADE_CIPOS];
    int agarrado;
    float bloqueio;
    float impulsoX;
    Texture2D textura;
    Texture2D nico;
} SistemaCipos;
void CarregarCipos(SistemaCipos *s);
void ReiniciarCipos(SistemaCipos *s);
void DescarregarCipos(SistemaCipos *s);
void AtualizarCipos(SistemaCipos *s, Jogador *j, float dt, float direcao, bool agarrar, bool soltar);
void DesenharCipos(const SistemaCipos *s);
void DesenharNicoNoCipo(const SistemaCipos *s, const Jogador *j);
#endif
