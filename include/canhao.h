#ifndef CANHAO_H
#define CANHAO_H

#include <stdbool.h>
#include "raylib.h"
#include "tiros.h"

#define MAX_CANHOES 8

typedef enum EstadoCanhao {
    CANHAO_PARADO,      // esperando um evento pedir disparo
    CANHAO_CARREGANDO,  // aviso visivel antes do tiro (da tempo de desviar)
    CANHAO_INTERVALO    // acabou de atirar; nao aceita outro disparo ainda
} EstadoCanhao;

typedef struct Canhao {
    Rectangle corpo;
    int direcao;         // 1 atira para a direita, -1 para a esquerda
    EstadoCanhao estado;
    float timer;         // quanto falta no estado atual
    float velocidadeTiro;
} Canhao;

// Os canhoes de uma fase juntos com os pedidos de disparo, para o
// main.c e o reinicio nunca esquecerem uma das partes.
typedef struct CanhoesDaFase {
    Canhao lista[MAX_CANHOES];
    bool pedidos[MAX_CANHOES];   // marcados pela ExecutarEventos
    int quantidade;              // 0 nas fases sem canhao
} CanhoesDaFase;

Canhao CriarCanhao(float x, float chaoY, int direcao);

// Posiciona os canhoes da fase (so o navio tem). Chamar ao carregar a fase.
// O indice de cada canhao e o mesmo usado nos eventos (eventos_fases.c).
void MontarCanhoesDaFase(CanhoesDaFase *canhoes, int faseId);

// Le os pedidos que a ExecutarEventos marcou em disparoCanhao[i]:
// canhao parado com pedido -> comeca o aviso e limpa a marca.
// Se o canhao estiver ocupado, a marca espera ele ficar livre.
// Retorna quantos tiros sairam neste quadro.
int AtualizarCanhoes(Canhao canhoes[], int quantidade, bool disparoCanhao[],
                     SistemaTiros *tiros, float dt);

// Reinicio da tentativa: todos parados e sem pedidos pendentes.
void ReiniciarCanhoes(Canhao canhoes[], int quantidade, bool disparoCanhao[]);

void DesenharCanhoes(const Canhao canhoes[], int quantidade);

#endif
