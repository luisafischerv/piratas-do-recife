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

// Atalhos para cadastrar (eventos_fases.c).
bool CadastrarSurgimentoMarujo(SequenciaEventos *seq, float instante, float x, float chaoY,
                               float limiteEsquerdo, float limiteDireito);
bool CadastrarDisparoCanhao(SequenciaEventos *seq, float instante, int indiceCanhao);

// Monta os eventos de uma fase: cadastra, ordena (Insertion Sort) e zera o relogio.
// Chamar UMA vez ao carregar a fase. Fase sem eventos fica com a sequencia vazia.
void MontarEventosDaFase(SequenciaEventos *seq, int faseId);

#endif