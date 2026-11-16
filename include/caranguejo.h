#ifndef CARANGUEJO_H
#define CARANGUEJO_H

#include "inimigos.h"
#include "jogador.h"

#define CARANGUEJO_SAUDE             2
#define CARANGUEJO_TEMPO_PREPARACAO  0.6f

Inimigo CriarCaranguejo(float x, float chaoY, float limiteEsquerdo, float limiteDireito);
int AtualizarCaranguejo(Inimigo *caranguejo, const Jogador *jogador, float dt);

#endif
