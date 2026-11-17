    #include <math.h>
#include "raylib.h"
#include "inimigos_ia.h"
#include "marujo.h"
#include "caranguejo.h"
#include "interface.h"
#include "tiros.h"

static Texture2D texturaMarujo;
static Texture2D texturaAndar;
static Texture2D texturaAtaque;
static Texture2D texturaAtirador;
static Texture2D texturaAtiradorAndar;

void CarregarVisualInimigos(void)
{
    texturaMarujo = LoadTexture("assets/images/marujo/marujo_proposta.png");
    texturaAndar = LoadTexture("assets/images/marujo/marujo_andar.png");
    texturaAtaque = LoadTexture("assets/images/marujo/marujo_ataque.png");
    texturaAtirador = LoadTexture("assets/images/atirador/atirador_sheet.png");
    texturaAtiradorAndar = LoadTexture("assets/images/atirador/atirador_andar.png");
    if (texturaMarujo.id) SetTextureFilter(texturaMarujo, TEXTURE_FILTER_BILINEAR);
    if (texturaAndar.id) SetTextureFilter(texturaAndar, TEXTURE_FILTER_BILINEAR);
    if (texturaAtaque.id) SetTextureFilter(texturaAtaque, TEXTURE_FILTER_BILINEAR);
    if (texturaAtirador.id) SetTextureFilter(texturaAtirador, TEXTURE_FILTER_BILINEAR);
    if (texturaAtiradorAndar.id) SetTextureFilter(texturaAtiradorAndar, TEXTURE_FILTER_BILINEAR);
}

void DescarregarVisualInimigos(void)
{
    if (texturaMarujo.id) UnloadTexture(texturaMarujo);
    if (texturaAndar.id) UnloadTexture(texturaAndar);
    if (texturaAtaque.id) UnloadTexture(texturaAtaque);
    if (texturaAtirador.id) UnloadTexture(texturaAtirador);
    if (texturaAtiradorAndar.id) UnloadTexture(texturaAtiradorAndar);
    texturaMarujo.id = 0;
    texturaAndar.id = texturaAtaque.id = 0;
    texturaAtirador.id = 0;
    texturaAtiradorAndar.id = 0;
}

static void DesenharMarujo(const Inimigo *m)
{
    float topoMarujo = m->corpo.y;
    if (texturaMarujo.id) {
        int quadro = 0;
        Texture2D textura = texturaMarujo;
        if (!m->aguardaProximidade && (m->estado == ESTADO_PATRULHA || m->estado == ESTADO_APROXIMACAO) && texturaAndar.id) {
            textura = texturaAndar;
            quadro = (int)(GetTime() * (m->estado == ESTADO_APROXIMACAO ? 10.0 : 7.0)) % 8;
        } else if (texturaAtaque.id && m->estado == ESTADO_PREPARACAO) {
            textura = texturaAtaque;
            quadro = (int)((0.45f - m->timerEstado) / 0.45f * 4.0f);
            if (quadro < 0) quadro = 0;
            if (quadro > 3) quadro = 3;
        } else if (texturaAtaque.id && m->estado == ESTADO_ATAQUE) {
            textura = texturaAtaque;
            quadro = m->timerEstado > 0.075f ? 4 : 5;
        } else if (texturaAtaque.id && m->estado == ESTADO_RECUPERACAO) {
            textura = texturaAtaque;
            quadro = m->timerEstado > 0.25f ? 6 : 7;
        }
        if (m->tempoDano > 0.0f) { textura = texturaMarujo; quadro = 7; }
        float largura = (float)textura.width / 4.0f;
        float altura = (float)textura.height / 2.0f;
        // A folha original olha para a esquerda.
        Rectangle origem = { (quadro % 4) * largura, (quadro / 4) * altura,
                             m->direcao > 0 ? -largura : largura, altura };
        float alturaVisual = 140.0f;
        float larguraVisual = alturaVisual * largura / altura;
        Rectangle destino = { m->corpo.x + m->corpo.width / 2.0f - larguraVisual / 2.0f,
                              m->corpo.y + m->corpo.height - alturaVisual * 0.96f,
                              larguraVisual, alturaVisual };
        topoMarujo = destino.y;
        DrawTexturePro(textura, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
    } else {
        DrawRectangleRec(m->corpo, MAROON);
    }

    const int pixel = 2;
    const int larguraCoracao = 8 * pixel;
    const int alturaCoracao = 7 * pixel;
    const int espaco = 4;
    const int passo = larguraCoracao + espaco;
    const int larguraTotal = MARUJO_SAUDE * passo - espaco;

    float centroMarujo = m->corpo.x + m->corpo.width / 2.0f;
    int inicioX = (int)(centroMarujo - larguraTotal / 2.0f);
    int yCoracoes = (int)topoMarujo - alturaCoracao - 3;

    for (int i = 0; i < MARUJO_SAUDE; i++) {
        DesenharCoracao(inicioX + i * passo, yCoracoes, pixel, i < m->saude);
    }
}

static void DesenharCaranguejo(const Inimigo *c)
{
    Rectangle r = c->corpo;
    Color casco = (Color){ 196, 72, 48, 255 };
    Color escuro = (Color){ 90, 28, 20, 255 };
    float progresso = 0.0f;
    float garraY = r.y + r.height * 0.35f;

    if (c->estado == ESTADO_PREPARACAO) {
        progresso = 1.0f - c->timerEstado / CARANGUEJO_TEMPO_PREPARACAO;
        if (progresso < 0.0f) progresso = 0.0f;
        if (progresso > 1.0f) progresso = 1.0f;
        // Treme cada vez mais, fica laranja e ergue as garras.
        r.x += sinf((float)GetTime() * 70.0f) * 3.0f * progresso;
        casco = (Color){ 196 + (unsigned char)(59 * progresso), 72 + (unsigned char)(80 * progresso), 48, 255 };
        garraY -= 14.0f * progresso;
    } else if (c->estado == ESTADO_RECUPERACAO) {
        casco = (Color){ 150, 60, 45, 255 };
    }
    if (c->tempoDano > 0.0f) casco = WHITE;

    float cx = r.x + r.width / 2.0f;
    float base = r.y + r.height;

    for (int i = -1; i <= 1; i += 2)
        for (int k = 0; k < 3; k++) {
            float px = cx + i * (10.0f + k * 8.0f);
            DrawLineEx((Vector2){ px, base - 12 }, (Vector2){ px + i * 8.0f, base }, 3.0f, escuro);
        }
    DrawEllipse((int)cx, (int)(base - 16), r.width / 2.0f, 14, casco);
    DrawEllipseLines((int)cx, (int)(base - 16), r.width / 2.0f, 14, escuro);
    for (int i = -1; i <= 1; i += 2) {
        Vector2 garra = { cx + i * (r.width / 2.0f + 4.0f), garraY };
        DrawLineEx((Vector2){ cx + i * 18.0f, base - 18 }, garra, 4.0f, escuro);
        DrawCircleV(garra, 7.0f, casco);
        DrawCircleLinesV(garra, 7.0f, escuro);
    }
    float olhoX = cx + c->direcao * 8.0f;
    DrawLineEx((Vector2){ olhoX - 5, base - 26 }, (Vector2){ olhoX - 5, base - 36 }, 2.0f, escuro);
    DrawLineEx((Vector2){ olhoX + 5, base - 26 }, (Vector2){ olhoX + 5, base - 36 }, 2.0f, escuro);
    DrawCircle((int)olhoX - 5, (int)(base - 37), 3.0f, RAYWHITE);
    DrawCircle((int)olhoX + 5, (int)(base - 37), 3.0f, RAYWHITE);

    if (c->estado == ESTADO_PREPARACAO) {
        DrawText("!", (int)cx - 5, (int)(r.y - 48), 28, ORANGE);
        float y = base - 6;
        float x0 = c->direcao > 0 ? r.x + r.width + 10 : r.x - 10;
        float x1 = x0 + c->direcao * 90.0f * progresso;
        DrawLineEx((Vector2){ x0, y }, (Vector2){ x1, y }, 3.0f, ORANGE);
    } else if (c->estado == ESTADO_ATAQUE) {
        // Rastro de velocidade atras do caranguejo.
        float tras = c->direcao > 0 ? r.x : r.x + r.width;
        for (int i = 0; i < 3; i++) {
            float y = r.y + 8 + i * 9;
            DrawLineEx((Vector2){ tras, y }, (Vector2){ tras - c->direcao * (24.0f + i * 6), y },
                       2.0f, Fade(RAYWHITE, 0.7f));
        }
    }

    const int pixel = 2, passo = 8 * pixel + 4;
    int inicioX = (int)(cx - (CARANGUEJO_SAUDE * passo - 4) / 2.0f);
    for (int i = 0; i < CARANGUEJO_SAUDE; i++)
        DesenharCoracao(inicioX + i * passo, (int)(r.y - 60), pixel, i < c->saude);
}

void DesenharInimigos(const ListaInimigos *lista)
{
    for (const NoInimigo *no = lista->inicio; no != NULL; no = no->proximo) {
        switch (no->dados.tipo) {
        case INIMIGO_MARUJO:
            DesenharMarujo(&no->dados);
            break;
        case INIMIGO_ATIRADOR:
            if (texturaAtirador.id) {
                const Inimigo *at = &no->dados;
                int quadro = 0;
                Texture2D textura = texturaAtirador;
                if (at->estado == ESTADO_PREPARACAO) {
                    quadro = (int)((0.8f - at->timerEstado) * 5.0f);
                    if (quadro < 0) quadro = 0;
                    if (quadro > 3) quadro = 3;
                } else if (at->golpeJaAcertou && at->estado == ESTADO_RECUPERACAO) {
                    if (at->timerEstado > 1.38f) quadro = 4;
                    else if (at->timerEstado > 1.15f) quadro = 5;
                    else if (at->timerEstado > 0.9f) quadro = 6;
                    else quadro = 7;
                }
                if (at->caminhando && texturaAtiradorAndar.id) {
                    textura = texturaAtiradorAndar;
                    quadro = (int)(at->tempoCaminhada * 6.0f) % 8;
                }
                float w = (float)textura.width / 4.0f;
                float h = (float)textura.height / 2.0f;
                Rectangle origem = { (quadro % 4) * w, (quadro / 4) * h,
                                     at->direcao > 0 ? -w : w, h };
                float altura = 140.0f, largura = altura * w / h;
                Rectangle destino = { at->corpo.x + at->corpo.width / 2.0f - largura / 2.0f,
                                      at->corpo.y + at->corpo.height - altura * 0.96f,
                                      largura, altura };
                DrawTexturePro(textura, origem, destino, (Vector2){0, 0}, 0.0f,
                               at->tempoDano > 0.0f ? RED : WHITE);
            } else {
                DrawRectangleRec(no->dados.corpo, no->dados.tempoDano > 0 ? WHITE : DARKPURPLE);
            }
            break;
        case INIMIGO_CARANGUEJO:
            DesenharCaranguejo(&no->dados);
            break;
        }
    }
}
