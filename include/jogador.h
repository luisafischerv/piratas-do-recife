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
    Texture2D texturaSaltoDiagonal;
    Texture2D texturaAtaque;
    float tempoAtaqueVisual;
    bool saltoDiagonal;
    float tempoAnimacao;
    float preparoSalto;
    float tempoNoAr;
    float tempoPouso;
    int quadroAtual;
    bool andando;
    int direcao;           
} Jogador;

void InicializarJogador(Jogador *jogador);
void AtualizarJogador(Jogador *jogador, float dt);
void IniciarAnimacaoAtaque(Jogador *jogador);
void DescarregarJogador(Jogador *jogador);
void DesenharJogador(const Jogador *jogador);

#endif
