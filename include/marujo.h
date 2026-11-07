#ifndef MARUJO_H
#define MARUJO_H

#include "inimigos.h"
#include "jogador.h"

Inimigo CriarMarujo(float x, float chaoY, float limiteEsquerdo, float limiteDireito);
Rectangle AreaGolpeMarujo(const Inimigo *inimigo);
int AtualizarMarujo(Inimigo *inimigo, const Jogador *jogador, float dt);

#endif