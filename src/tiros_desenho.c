#include "tiros.h"

static Texture2D texturaBala;

void CarregarVisualTiros(void)
{
    texturaBala = LoadTexture("assets/images/projeteis/bala.png");
    if (texturaBala.id) SetTextureFilter(texturaBala, TEXTURE_FILTER_BILINEAR);
}

void DescarregarVisualTiros(void)
{
    if (texturaBala.id) UnloadTexture(texturaBala);
    texturaBala.id = 0;
}

void DesenharTiros(const SistemaTiros *s)
{
    for (int i = 0; i < MAX_TIROS; i++) {
        const Tiro *tiro = &s->tiros[i];
        if (!tiro->ativo) continue;
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
