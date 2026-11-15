#ifndef EVENTOS_H
#define EVENTOS_H

#include <stdbool.h>
#include "inimigos.h"

#define MAX_EVENTOS 64

typedef enum TipoEvento {
    EVENTO_SURGIR_MARUJO,
    EVENTO_DISPARO_CANHAO
} TipoEvento;

typedef struct Evento {
    float instante;
    TipoEvento tipo;
    float x, chaoY;
    float limiteEsquerdo, limiteDireito;
    int indiceCanhao;
    int ordemCadastro;
    bool executado;
} Evento;

typedef struct SequenciaEventos {
    Evento eventos[MAX_EVENTOS];
    int quantidade;
    int proximo;
    float relogio;
} SequenciaEventos;

void InicializarSequencia(SequenciaEventos *seq);
bool CadastrarEvento(SequenciaEventos *seq, Evento evento);
void OrdenarEventos(Evento v[], int n);
int ExecutarEventos(SequenciaEventos *seq, float dt, ListaInimigos *lista,
                    bool disparoCanhao[], int maxCanhoes);
void ReiniciarEventos(SequenciaEventos *seq);

#endif