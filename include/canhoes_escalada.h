#ifndef CANHOES_ESCALADA_H
#define CANHOES_ESCALADA_H
#include "canhao.h"
#include "eventos.h"

#define CANHOES_ESCALADA_LATERAIS 12
#define CANHOES_ESCALADA_TOTAL (CANHOES_ESCALADA_LATERAIS + 3)

typedef struct CanhoesEscalada {
    Canhao laterais[CANHOES_ESCALADA_LATERAIS];
    bool pedidos[CANHOES_ESCALADA_TOTAL];
    SequenciaEventos eventos;
    float avisoVertical[3];
    float yBala[3];
    bool balaAtiva[3];
    int trecho;
    int rodada;
    bool terminou;
} CanhoesEscalada;
void ReiniciarCanhoesEscalada(CanhoesEscalada *s);
int AtualizarCanhoesEscalada(CanhoesEscalada *s, SistemaTiros *tiros, ListaInimigos *inimigos, const Jogador *j, float dt);
void CarregarVisualEscalada(void);
void DescarregarVisualEscalada(void);
void DesenharCanhoesEscalada(const CanhoesEscalada *s);
#endif
