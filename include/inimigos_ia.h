#ifndef INIMIGOS_IA_H
#define INIMIGOS_IA_H

#include "inimigos.h"

// Auxiliares comuns a todos os inimigos
bool RetangulosSobrepostos(Rectangle a, Rectangle b);
float CentroXRetangulo(Rectangle r);
float CentroYRetangulo(Rectangle r);
void MoverInimigoNosLimites(Inimigo *inimigo, float deslocamento);
void MudarEstadoInimigo(Inimigo *inimigo, EstadoInimigo novo, float duracao);
int AtualizarInimigos(ListaInimigos *lista, const Jogador *jogador, float dt);

#endif