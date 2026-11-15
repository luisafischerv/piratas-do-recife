#include <math.h>
#include "marujo.h"
#include "inimigos_ia.h"

// Valores EXPERIMENTAIS
#define MARUJO_LARGURA  44.0f
#define MARUJO_ALTURA   64.0f
#define MARUJO_VEL_PATRULHA      95.0f   // px por segundo
#define MARUJO_VEL_APROXIMACAO   150.0f
#define MARUJO_PERCEPCAO         360.0f  // distancia em que nota o Nico
#define MARUJO_DESISTENCIA       440.0f  // se o Nico passar disso, volta a patrulhar
#define MARUJO_DIF_ALTURA_MAX    90.0f   // ignora o Nico muito acima ou abaixo
#define MARUJO_ALCANCE_GOLPE     36.0f
#define MARUJO_TEMPO_PREPARACAO  0.45f   // segundos de aviso antes do golpe
#define MARUJO_TEMPO_ATAQUE      0.15f   // segundos em que o golpe causa dano
#define MARUJO_TEMPO_RECUPERACAO 0.50f   // brecha para o jogador contra-atacar
#define MARUJO_DANO              1

Inimigo CriarMarujo(float x, float chaoY, float limiteEsquerdo, float limiteDireito)
{
    Inimigo m = {0};
    m.tipo = INIMIGO_MARUJO;
    m.corpo = (Rectangle){ x, chaoY - MARUJO_ALTURA, MARUJO_LARGURA, MARUJO_ALTURA };
    m.direcao = -1;
    m.saude = MARUJO_SAUDE;
    m.estado = ESTADO_PATRULHA;
    m.limiteEsquerdo = limiteEsquerdo;
    m.limiteDireito = limiteDireito;
    return m;
}

Rectangle AreaGolpeMarujo(const Inimigo *m)
{
    float altura = m->corpo.height * 0.6f;
    float y = m->corpo.y + m->corpo.height * 0.2f;
    float x = (m->direcao > 0) ? m->corpo.x + m->corpo.width
                               : m->corpo.x - MARUJO_ALCANCE_GOLPE;
    return (Rectangle){ x, y, MARUJO_ALCANCE_GOLPE, altura };
}

int AtualizarMarujo(Inimigo *m, const Jogador *jogador, float dt)
{
    int dano = 0;

    // Onde o Nico esta em relacao ao marujo?
    float dx = CentroXRetangulo(jogador->corpo) - CentroXRetangulo(m->corpo);
    float dy = CentroYRetangulo(jogador->corpo) - CentroYRetangulo(m->corpo);
    float distancia = fabsf(dx);
    bool mesmaAltura = fabsf(dy) < MARUJO_DIF_ALTURA_MAX;
    // Espaco vazio entre os dois corpos (0 = encostados).
    float vao = distancia - (m->corpo.width + jogador->corpo.width) / 2.0f;
    // Considera o alcance do golpe alem da area de patrulha.
    float alcanceInicioGolpe = MARUJO_ALCANCE_GOLPE * 0.8f;
    bool jogadorAlcancavel = jogador->corpo.x + jogador->corpo.width >= m->limiteEsquerdo - alcanceInicioGolpe &&
                             jogador->corpo.x <= m->limiteDireito + alcanceInicioGolpe;

    if (m->timerEstado > 0.0f) m->timerEstado -= dt;

    switch (m->estado) {
    case ESTADO_PATRULHA:
        MoverInimigoNosLimites(m, m->direcao * MARUJO_VEL_PATRULHA * dt);
        if (m->corpo.x <= m->limiteEsquerdo) m->direcao = 1;
        if (m->corpo.x + m->corpo.width >= m->limiteDireito) m->direcao = -1;

        if (distancia < MARUJO_PERCEPCAO && mesmaAltura && jogadorAlcancavel)
            MudarEstadoInimigo(m, ESTADO_APROXIMACAO, 0.0f);
        break;

    case ESTADO_APROXIMACAO:
        m->direcao = (dx >= 0.0f) ? 1 : -1;
        if (distancia > MARUJO_DESISTENCIA || !mesmaAltura || !jogadorAlcancavel) {
            // Se Nico saiu da area, volta a percorrer o cais em vez de ficar parado na borda.
            if (!jogadorAlcancavel) m->direcao = dx < 0.0f ? 1 : -1;
            MudarEstadoInimigo(m, ESTADO_PATRULHA, 0.0f);
        } else if (vao <= MARUJO_ALCANCE_GOLPE * 0.8f) {
            MudarEstadoInimigo(m, ESTADO_PREPARACAO, MARUJO_TEMPO_PREPARACAO);
        } else {
            MoverInimigoNosLimites(m, m->direcao * MARUJO_VEL_APROXIMACAO * dt);
        }
        break;

    case ESTADO_PREPARACAO:
        if (m->timerEstado <= 0.0f) {
            m->golpeJaAcertou = false;
            MudarEstadoInimigo(m, ESTADO_ATAQUE, MARUJO_TEMPO_ATAQUE);
        }
        break;

    case ESTADO_ATAQUE:
        if (!m->golpeJaAcertou &&
            RetangulosSobrepostos(AreaGolpeMarujo(m), jogador->corpo)) {
            dano = MARUJO_DANO;
            m->golpeJaAcertou = true;
        }
        if (m->timerEstado <= 0.0f)
            MudarEstadoInimigo(m, ESTADO_RECUPERACAO, MARUJO_TEMPO_RECUPERACAO);
        break;

    case ESTADO_RECUPERACAO:
        if (m->timerEstado <= 0.0f)
            MudarEstadoInimigo(m, ESTADO_APROXIMACAO, 0.0f);
        break;
    }

    return dano;
}
