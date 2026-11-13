#include "colisao.h"

#define TOLERANCIA 1.0f

static bool Sobrepoe(Rectangle a, Rectangle b) {
    return a.x < b.x + b.width && a.x + a.width > b.x && a.y < b.y + b.height && a.y + a.height > b.y;
}

void ResolverColisoesCenario(Jogador *j, const Fase *f, Rectangle anterior) {
    j->estaNoChao = false;

    for (int i = 0; i < f->qtdPlataformas; i++) {
        const Plataforma *plat = &f->plataformas[i];
        if (!plat->ativa) continue;

        Rectangle p = plat->area;
        if (!Sobrepoe(j->corpo, p)) continue;

        float baseAnterior = anterior.y + anterior.height;
        if (baseAnterior <= p.y + TOLERANCIA) {
            j->corpo.y = p.y - j->corpo.height;
            if (j->velocidadeY > 0.0f) j->velocidadeY = 0.0f;
            j->estaNoChao = true;
        } else if (anterior.y >= p.y + p.height - TOLERANCIA) {
            j->corpo.y = p.y + p.height;
            if (j->velocidadeY < 0.0f) j->velocidadeY = 0.0f;
        } else if (anterior.x + anterior.width <= p.x + TOLERANCIA) {
            j->corpo.x = p.x - j->corpo.width;
        } else if (anterior.x >= p.x + p.width - TOLERANCIA) {
            j->corpo.x = p.x + p.width;
        }
    }
}