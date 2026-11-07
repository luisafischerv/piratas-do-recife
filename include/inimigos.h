#ifndef INIMIGOS_H
#define INIMIGOS_H

#include <stddef.h> // traz o NULL
#include "raylib.h"
#include "jogador.h"

typedef enum TipoInimigo {
    INIMIGO_MARUJO,
    INIMIGO_ATIRADOR,
    INIMIGO_CARANGUEJO
} TipoInimigo;

typedef enum EstadoInimigo {
    ESTADO_PATRULHA,
    ESTADO_APROXIMACAO,
    ESTADO_PREPARACAO,
    ESTADO_ATAQUE,
    ESTADO_RECUPERACAO
} EstadoInimigo;

typedef struct Inimigo {
    TipoInimigo tipo;
    Rectangle corpo;
    int direcao; // 1 direita e -1 esquerda 
    int saude; // quantos golpes ainda aguenta
    EstadoInimigo estado;
    float timerEstado; // quanto tempo falta no estado atual
    float limiteEsquerdo;
    float limiteDireito;
} Inimigo;

typedef struct NoInimigo {
    Inimigo dados;
    struct NoInimigo *anterior;
    struct NoInimigo *proximo;
} NoInimigo;

typedef struct ListaInimigos {
    NoInimigo *inicio;
    NoInimigo *fim;
    int quantidade;
} ListaInimigos;


void InicializarLista(ListaInimigos *lista);
NoInimigo *InserirInimigo(ListaInimigos *lista, Inimigo dados);
void RemoverInimigo(ListaInimigos *lista, NoInimigo *no);
void LiberarInimigos(ListaInimigos *lista);


#endif