#ifndef COLISAO_H
#define COLISAO_H

#include "raylib.h"
#include "jogador.h"
#include "fase.h"

void ResolverColisoesCenario(Jogador *jogador, const Fase *fase, Rectangle anterior);

#endif