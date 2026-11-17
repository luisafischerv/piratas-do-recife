#ifndef CORDAS_H
#define CORDAS_H
#include "jogador.h"

#define NUM_CORDAS_ESCALADA 3
typedef struct SistemaCordas {
    int agarrada;
    float bloqueio;
    float tempoAnimacao;
    Texture2D textura;
    Texture2D nico;
} SistemaCordas;
void CarregarCordas(SistemaCordas *s);
void ReiniciarCordas(SistemaCordas *s);
void SoltarCordaPorDano(SistemaCordas *s, Jogador *j);
void DescarregarCordas(SistemaCordas *s);
void AtualizarCordas(SistemaCordas *s, Jogador *j, float dt, int vertical, int horizontal, bool agarrar, bool saltar);
void DesenharCordas(const SistemaCordas *s);
void DesenharNicoNaCorda(const SistemaCordas *s, const Jogador *j);
#endif
