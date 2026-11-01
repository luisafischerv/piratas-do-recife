#ifndef JOGADOR_H
#define JOGADOR_H

#include "raylib.h"

typedef struct Jogador {
    Rectangle corpo;      
    float velocidadeY;     
    bool estaNoChao;
    int direcao;           
} Jogador;

void InicializarJogador(Jogador *jogador);
void AtualizarJogador(Jogador *jogador, float dt);
void DesenharJogador(const Jogador *jogador);

#endif
