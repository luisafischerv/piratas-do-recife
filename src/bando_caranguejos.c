#include "bando_caranguejos.h"

#define LARGURA_ARENA 1280.0f


void ReiniciarBando(BandoCaranguejos *b) {
    *b = (BandoCaranguejos){0};
    b->espera = 5.0f;
    b->direcao = -1;
}

static int DirecaoDoCaranguejo(const BandoCaranguejos *b, int i) {
    return i < 2 || b->quantidade == 2 ? b->direcao : (i / 2 % 2 ? -b->direcao : b->direcao);
}

int AtualizarBando(BandoCaranguejos *b, const Jogador *j, int estagio, float dt) {
    if (!b->ativo) {
        b->quantidade = estagio * 2;
        b->espera -= dt;
        if (b->espera > 0) return 0;
        b->ativo = true;
        for (int i = 0; i < b->quantidade; i++) {
            float atraso = (i / 2) * 210.0f + (i % 2) * 55.0f;
            float x = DirecaoDoCaranguejo(b, i) < 0 ? LARGURA_ARENA + atraso : -46.0f - atraso;
            b->corpos[i] = (Rectangle){x, 568, 46, 32};
            b->atingiu[i] = false;
        }
    }
    float velocidade = 210.0f + 25.0f * (b->quantidade / 2 - 1);
    int dano = 0;
    bool algumNaTela = false;
    for (int i = 0; i < b->quantidade; i++) {
        Rectangle *r = &b->corpos[i];
        float anterior = r->x;
        int direcao = DirecaoDoCaranguejo(b, i);
        r->x += direcao * velocidade * dt;
        Rectangle caminho = {direcao < 0 ? r->x : anterior, r->y,
                             r->width + velocidade * dt, r->height};
        if (!b->atingiu[i] && CheckCollisionRecs(caminho, j->corpo)) {
            b->atingiu[i] = true;
            dano = 1;
        }
        if (direcao < 0 ? r->x + r->width >= 0 : r->x <= LARGURA_ARENA)
            algumNaTela = true;
    }
    if (!algumNaTela) {
        b->ativo = false;
        b->espera = estagio >= 3 ? 2.4f : estagio == 2 ? 3.6f : 5.0f;
        b->direcao = -b->direcao;
    }
    return dano;
}

void DesenharBando(const BandoCaranguejos *b) {
    if (!b->ativo) return;
    for (int i = 0; i < b->quantidade; i++) {
        Rectangle r = b->corpos[i];
        float cx = r.x + r.width / 2, base = r.y + r.height;
        for (int lado = -1; lado <= 1; lado += 2) {
            for (int k = 0; k < 3; k++)
                DrawLineEx((Vector2){cx + lado * (8 + k * 6), base - 10},
                           (Vector2){cx + lado * (14 + k * 6), base}, 3, MAROON);
            DrawCircle((int)(cx + lado * 24), (int)(base - 19), 6, MAROON);
        }
        DrawEllipse((int)cx, (int)(base - 14), 22, 13, (Color){196, 72, 48, 255});
        DrawCircle((int)cx - 6, (int)(base - 26), 3, RAYWHITE);
        DrawCircle((int)cx + 6, (int)(base - 26), 3, RAYWHITE);
    }
}
