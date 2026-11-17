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

// Navio (fase 3) - POSICOES PROVISORIAS: ajustar quando a Pessoa 2 criar o convés.
// Supoe convés continuo com o chao em y = 620. Os canhoes ficam a frente do
// caminho do Nico e atiram para a esquerda, na direcao de onde ele vem.
#define NAVIO_CHAO_Y 620.0f

void MontarCanhoesDaFase(CanhoesDaFase *c, int faseId)
{
    c->quantidade = 0;
    for (int i = 0; i < MAX_CANHOES; i++) c->pedidos[i] = false;

    if (faseId == 3) {
        c->lista[0] = CriarCanhao(1150.0f, NAVIO_CHAO_Y, -1);   // indice 0
        c->lista[1] = CriarCanhao(2050.0f, NAVIO_CHAO_Y, -1);   // indice 1
        c->lista[2] = CriarCanhao(2950.0f, NAVIO_CHAO_Y, -1);   // indice 2
        c->quantidade = 3;
    }
}
