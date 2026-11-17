#include "canhao.h"

static Texture2D texturaCanhao;

void CarregarVisualCanhoes(void) {
    texturaCanhao = LoadTexture("assets/images/propostas_combate/canhao_horizontal_v3.png");
    if (texturaCanhao.id) SetTextureFilter(texturaCanhao, TEXTURE_FILTER_BILINEAR);
}

void DescarregarVisualCanhoes(void) {
    if (texturaCanhao.id) UnloadTexture(texturaCanhao);
    texturaCanhao = (Texture2D){0};
}

void DesenharCanhoes(const Canhao canhoes[], int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        const Canhao *c = &canhoes[i];
        int quadro = 0;
        if (c->estado == CANHAO_INTERVALO) {
            quadro = c->timer > 0.65f ? 1 : c->timer > 0.4f ? 2 : 3;
        }
        if (!texturaCanhao.id) {
            DrawRectangleRec(c->corpo, DARKGRAY);
            continue;
        }
        float largura = texturaCanhao.width / 4.0f;
        Rectangle origem = {quadro * largura, 0, c->direcao > 0 ? largura : -largura, texturaCanhao.height};
        Rectangle destino = {c->corpo.x - 60, c->corpo.y - 65, 180, 165};
        DrawTexturePro(texturaCanhao, origem, destino, (Vector2){0, 0}, 0, WHITE);
    }
}
