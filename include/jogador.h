#ifndef JOGADOR_H
#define JOGADOR_H

#include "raylib.h"

typedef struct Jogador {
    Rectangle corpo;      
    float velocidadeY;     
    bool estaNoChao;
    Texture2D texturaCorrida;
    Texture2D texturaParado;
    Texture2D texturaSalto;
    float tempoAnimacao;
    int quadroAtual;
    bool andando;
    int direcao;           
} Jogador;

void InicializarJogador(Jogador *jogador);
void AtualizarJogador(Jogador *jogador, float dt);
void DescarregarJogador(Jogador *jogador);
void DesenharJogador(const Jogador *jogador);

#endif
