#ifndef TIROS_H
#define TIROS_H

#include "fase.h"
#include "inimigos.h"

#define ATIRADOR_SAUDE 3
#define MAX_TIROS 64
// A arte dos personagens ultrapassa o topo do colisor usado para movimento.
#define ALTURA_TIRO_RELATIVA (-0.10f)

typedef struct Tiro {
    Rectangle area;
    float velocidade;
    bool ativo;
    bool doJogador;
    bool deCanhao;
} Tiro;

typedef struct SistemaTiros {
    Tiro tiros[MAX_TIROS];
    float recarga;
} SistemaTiros;

void InicializarTiros(SistemaTiros *sistema);
bool AtualizarPistola(SistemaTiros *sistema, const Jogador *jogador, bool pressionado, float dt);
// Tiro horizontal que fere o Nico, saindo da frente de 'origem' (usado pelos canhoes).
bool DispararTiroInimigo(SistemaTiros *sistema, Rectangle origem, int direcao);
bool DispararTiroInimigoComVelocidade(SistemaTiros *sistema, Rectangle origem, int direcao, float velocidade);
void AtualizarAtiradores(SistemaTiros *sistema, ListaInimigos *inimigos, const Jogador *jogador, float dt);
int AtualizarTiros(SistemaTiros *sistema, const Fase *fase, ListaInimigos *inimigos, const Jogador *jogador, float dt);
void DesenharTiros(const SistemaTiros *sistema);
void CarregarVisualTiros(void);
void DescarregarVisualTiros(void);
Inimigo CriarAtirador(float x, float chaoY);

#endif
