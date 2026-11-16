#include <math.h>
#include "caranguejo.h"
#include "inimigos_ia.h"

// Valores EXPERIMENTAIS
#define CARANGUEJO_LARGURA           56.0f
#define CARANGUEJO_ALTURA            36.0f
#define CARANGUEJO_VEL_PATRULHA      60.0f
#define CARANGUEJO_PERCEPCAO         320.0f
#define CARANGUEJO_DIF_ALTURA_MAX    90.0f
#define CARANGUEJO_VEL_INVESTIDA     420.0f
#define CARANGUEJO_TEMPO_INVESTIDA   0.6f
#define CARANGUEJO_TEMPO_RECUPERACAO 0.8f
#define CARANGUEJO_DANO              1

Inimigo CriarCaranguejo(float x, float chaoY, float limiteEsquerdo, float limiteDireito)
{
    Inimigo c = {0};
    c.tipo = INIMIGO_CARANGUEJO;
    c.corpo = (Rectangle){ x, chaoY - CARANGUEJO_ALTURA, CARANGUEJO_LARGURA, CARANGUEJO_ALTURA };
    c.direcao = -1;
    c.saude = CARANGUEJO_SAUDE;
    c.estado = ESTADO_PATRULHA;
    c.limiteEsquerdo = limiteEsquerdo;
    c.limiteDireito = limiteDireito;
    return c;
}

int AtualizarCaranguejo(Inimigo *c, const Jogador *j, float dt)
{
    int dano = 0;

    float dx = CentroXRetangulo(j->corpo) - CentroXRetangulo(c->corpo);
    float dy = CentroYRetangulo(j->corpo) - CentroYRetangulo(c->corpo);
    bool percebeu = fabsf(dx) < CARANGUEJO_PERCEPCAO && fabsf(dy) < CARANGUEJO_DIF_ALTURA_MAX;

    if (c->timerEstado > 0.0f) c->timerEstado -= dt;

    switch (c->estado) {
    case ESTADO_PATRULHA:
        MoverInimigoNosLimites(c, c->direcao * CARANGUEJO_VEL_PATRULHA * dt);
        if (c->corpo.x <= c->limiteEsquerdo) c->direcao = 1;
        if (c->corpo.x + c->corpo.width >= c->limiteDireito) c->direcao = -1;

        if (percebeu) {
            c->direcao = (dx >= 0.0f) ? 1 : -1;
            MudarEstadoInimigo(c, ESTADO_PREPARACAO, CARANGUEJO_TEMPO_PREPARACAO);
        }
        break;

    case ESTADO_PREPARACAO:
        if (c->timerEstado <= 0.0f) {
            c->golpeJaAcertou = false;
            MudarEstadoInimigo(c, ESTADO_ATAQUE, CARANGUEJO_TEMPO_INVESTIDA);
        }
        break;

    case ESTADO_ATAQUE: {
        float antes = c->corpo.x;
        MoverInimigoNosLimites(c, c->direcao * CARANGUEJO_VEL_INVESTIDA * dt);
        bool bateuNoLimite = c->corpo.x == antes;

        if (!c->golpeJaAcertou && RetangulosSobrepostos(c->corpo, j->corpo)) {
            dano = CARANGUEJO_DANO;
            c->golpeJaAcertou = true;
        }
        if (c->timerEstado <= 0.0f || bateuNoLimite)
            MudarEstadoInimigo(c, ESTADO_RECUPERACAO, CARANGUEJO_TEMPO_RECUPERACAO);
        break;
    }

    case ESTADO_RECUPERACAO:
        if (c->timerEstado <= 0.0f)
            MudarEstadoInimigo(c, ESTADO_PATRULHA, 0.0f);
        break;

    default:
        MudarEstadoInimigo(c, ESTADO_PATRULHA, 0.0f);
        break;
    }

    return dano;
}
