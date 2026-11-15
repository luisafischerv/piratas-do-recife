#ifndef TIROS_H
#define TIROS_H

#include "fase.h"
#include "inimigos.h"

#define MAX_TIROS 64

typedef struct Tiro {
    Rectangle area;
    float velocidade;
    bool ativo;
    bool doJogador;
} Tiro;

typedef struct SistemaTiros {
    Tiro tiros[MAX_TIROS];
    float recarga;
} SistemaTiros;

void InicializarTiros(SistemaTiros *sistema);
void AtualizarPistola(SistemaTiros *sistema, const Jogador *jogador, bool pressionado, float dt);
void AtualizarAtiradores(SistemaTiros *sistema, ListaInimigos *inimigos, const Jogador *jogador, float dt);
int AtualizarTiros(SistemaTiros *sistema, const Fase *fase, ListaInimigos *inimigos, const Jogador *jogador, float dt);
void DesenharTiros(const SistemaTiros *sistema);
Inimigo CriarAtirador(float x, float chaoY);

#endif
