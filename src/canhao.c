#include "canhao.h"

// Valores EXPERIMENTAIS
#define CANHAO_LARGURA       60.0f
#define CANHAO_ALTURA        40.0f
#define CANHAO_TEMPO_AVISO   1.0f   // segundos de brilho antes do tiro
#define CANHAO_TEMPO_INTERVALO 0.8f // descanso depois do tiro

Canhao CriarCanhao(float x, float chaoY, int direcao)
{
    Canhao c = {0};
    c.corpo = (Rectangle){ x, chaoY - CANHAO_ALTURA, CANHAO_LARGURA, CANHAO_ALTURA };
    c.direcao = (direcao >= 0) ? 1 : -1;
    c.estado = CANHAO_PARADO;
    return c;
}

// Atualiza um canhao. Retorna true no quadro em que ele atira.
static bool AtualizarUmCanhao(Canhao *c, bool *pedido, SistemaTiros *tiros, float dt)
{
    if (c->timer > 0.0f) c->timer -= dt;

    switch (c->estado) {
    case CANHAO_PARADO:
        if (*pedido) {
            *pedido = false;                     // pedido atendido
            c->estado = CANHAO_CARREGANDO;
            c->timer = CANHAO_TEMPO_AVISO;
        }
        break;

    case CANHAO_CARREGANDO:
        if (c->timer <= 0.0f) {
            DispararTiroInimigo(tiros, c->corpo, c->direcao);
            c->estado = CANHAO_INTERVALO;
            c->timer = CANHAO_TEMPO_INTERVALO;
            return true;
        }
        break;

    case CANHAO_INTERVALO:
        if (c->timer <= 0.0f) c->estado = CANHAO_PARADO;
        break;
    }
    return false;
}

int AtualizarCanhoes(Canhao canhoes[], int quantidade, bool disparoCanhao[],
                     SistemaTiros *tiros, float dt)
{
    int disparos = 0;
    for (int i = 0; i < quantidade; i++) {
        if (AtualizarUmCanhao(&canhoes[i], &disparoCanhao[i], tiros, dt))
            disparos++;
    }
    return disparos;
}

void ReiniciarCanhoes(Canhao canhoes[], int quantidade, bool disparoCanhao[])
{
    for (int i = 0; i < quantidade; i++) {
        canhoes[i].estado = CANHAO_PARADO;
        canhoes[i].timer = 0.0f;
        disparoCanhao[i] = false;
    }
}
