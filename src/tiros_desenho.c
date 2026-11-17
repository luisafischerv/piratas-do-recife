#include "tiros.h"

static Texture2D texturaBala;
static Texture2D texturaBalaCanhao;

void CarregarVisualTiros(void)
{
    texturaBalaCanhao = LoadTexture("assets/images/propostas_combate/bala_canhao.png");
    if (texturaBalaCanhao.id) SetTextureFilter(texturaBalaCanhao, TEXTURE_FILTER_BILINEAR);
    texturaBala = LoadTexture("assets/images/projeteis/bala.png");
    if (texturaBala.id) SetTextureFilter(texturaBala, TEXTURE_FILTER_BILINEAR);
}

void DescarregarVisualTiros(void)
{
    if (texturaBala.id) UnloadTexture(texturaBala);
    texturaBala.id = 0;
    if (texturaBalaCanhao.id) UnloadTexture(texturaBalaCanhao);
    texturaBalaCanhao.id = 0;
}

void DesenharTiros(const SistemaTiros *s)
{
    for (int i = 0; i < MAX_TIROS; i++) {
        const Tiro *tiro = &s->tiros[i];
        if (!tiro->ativo) continue;
        if (tiro->deCanhao && texturaBalaCanhao.id) {
            Rectangle origem = {430, 180, 680, 680};
            Rectangle destino = {tiro->area.x - 6, tiro->area.y - 8, 24, 24};
            DrawTexturePro(texturaBalaCanhao, origem, destino, (Vector2){0, 0}, 0, WHITE);
            continue;
        }
        if (!texturaBala.id) {
            DrawRectangleRec(tiro->area, tiro->doJogador ? GOLD : RED);
            continue;
        }
        // A imagem original tem margens; usa apenas a regiao central da bala.
        Rectangle origem = { 480, 230, 580, 580 };
        Rectangle destino = { tiro->area.x + tiro->area.width / 2.0f - 9.0f,
                              tiro->area.y + tiro->area.height / 2.0f - 9.0f,
                              18.0f, 18.0f };
        DrawTexturePro(texturaBala, origem, destino, (Vector2){0, 0}, 0.0f,
                       tiro->doJogador ? WHITE : (Color){255, 150, 150, 255});
    }
}
