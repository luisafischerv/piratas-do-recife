#ifndef COMBATE_H
#define COMBATE_H

#include <stdbool.h>
#include "raylib.h"
#include "jogador.h"
#include "inimigos.h"

// Estado da espada do Nico. Fica separado da struct Jogador (Pessoa 1).
typedef struct Espada {
    bool ativa;          // a area de dano existe neste quadro?
    float tempoAtivo;    // quanto falta para a area sumir
    float tempoRecarga;  // quanto falta para poder golpear de novo
    int idGolpe;         // numero do golpe atual (0 = nenhum ainda; 1o golpe = 1)
} Espada;

void InicializarEspada(Espada *espada);

// 'clicou' deve ser IsMouseButtonPressed(MOUSE_BUTTON_LEFT): so e true no
// quadro do clique, entao segurar o botao nao gera outro golpe.
// Retorna true no quadro em que um golpe novo comeca.
bool AtualizarAtaqueEspada(Espada *espada, bool clicou, float dt);

// Retangulo a frente do Nico, conforme jogador->direcao. Vale no chao e no ar.
Rectangle AreaEspada(const Jogador *jogador);

// Tira 1 de saude de cada inimigo dentro da area, no maximo uma vez por golpe.
// Retorna quantos inimigos foram atingidos neste quadro.
// Nao remove ninguem: os mortos saem da lista na AtualizarInimigos.
int VerificarAcertosEspada(const Espada *espada, const Jogador *jogador, ListaInimigos *lista);

void DesenharEspada(const Espada *espada, const Jogador *jogador);

#endif
