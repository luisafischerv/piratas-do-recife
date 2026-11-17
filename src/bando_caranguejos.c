#include "bando_caranguejos.h"

#define LARGURA_ARENA 1280.0f

static Texture2D texturaCaranguejo;

void CarregarVisualBando(void) {
    texturaCaranguejo = LoadTexture("assets/images/propostas_combate/caranguejo.png");
    if (texturaCaranguejo.id) SetTextureFilter(texturaCaranguejo, TEXTURE_FILTER_BILINEAR);
}

void DescarregarVisualBando(void) {
    if (texturaCaranguejo.id) UnloadTexture(texturaCaranguejo);
    texturaCaranguejo = (Texture2D){0};
}


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
        if (!texturaCaranguejo.id) {
            DrawEllipse((int)(r.x + r.width / 2), (int)(r.y + r.height / 2), 22, 13, MAROON);
            continue;
        }
        float largura = texturaCaranguejo.width / 4.0f;
        int quadro = (int)(GetTime() * 8.0) % 4;
        Rectangle origem = {quadro * largura, 0, DirecaoDoCaranguejo(b, i) > 0 ? largura : -largura, texturaCaranguejo.height};
        Rectangle destino = {r.x - 14, r.y - 27, r.width + 28, r.height + 40};
        DrawTexturePro(texturaCaranguejo, origem, destino, (Vector2){0, 0}, 0, WHITE);
    }
}
