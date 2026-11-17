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

Canhao CriarCanhao(float x, float chaoY, int direcao);

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
