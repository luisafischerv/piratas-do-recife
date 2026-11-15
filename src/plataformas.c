#include <math.h>
#include "fase.h"

#define TEMPO_AVISO_FRAGIL 0.6f

static bool NicoPisando(const Jogador *j, Rectangle p){
    float pesY = j->corpo.y + j->corpo.height;
    bool alinhadoX = j->corpo.x + j->corpo.width > p.x && j->corpo.x < p.x + p.width;
    return j->estaNoChao && alinhadoX && fabsf(pesY - p.y) < 2.0f;
}

void AtualizarPlataformasFrageis(Fase *f, const Jogador *j, float dt){
    for (int i = 0; i < f->qtdPlataformas; i++) {
        Plataforma *p = &f->plataformas[i];
        if (p->tipo != PLAT_FRAGIL) continue;

        if (p->estado == PLAT_NORMAL) {
            if (NicoPisando(j, p->area)) {
                p->estado = PLAT_AVISO;
                p->tempoAviso = TEMPO_AVISO_FRAGIL;
            }
        } else if (p->estado == PLAT_AVISO) {
            p->tempoAviso -= dt;
            if (p->tempoAviso <= 0.0f) {
                p->estado = PLAT_CAIDA;
                p->ativa = false;
            }
        }
    }
}

void RestaurarPlataformas(Fase *f){
    for (int i = 0; i < f->qtdPlataformas; i++){
        Plataforma *p = &f->plataformas[i];
        p->estado = PLAT_NORMAL;
        p->tempoAviso = 0.0f;
        p->ativa = true;
    }
}