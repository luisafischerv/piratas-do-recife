#include "raylib.h"
#include "inimigos_ia.h"
#include "marujo.h"
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

void DesenharInimigos(const ListaInimigos *lista)
{
    for (const NoInimigo *no = lista->inicio; no != NULL; no = no->proximo) {
        switch (no->dados.tipo) {
        case INIMIGO_MARUJO:
            DesenharMarujo(&no->dados);
            break;

        case INIMIGO_ATIRADOR: {
            const Inimigo *at = &no->dados;
            float topoAtirador = at->corpo.y;

            if (texturaAtirador.id) {
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
                topoAtirador = destino.y;
                DrawTexturePro(textura, origem, destino, (Vector2){0, 0}, 0.0f,
                               at->tempoDano > 0.0f ? RED : WHITE);
            } else {
                DrawRectangleRec(at->corpo, at->tempoDano > 0 ? WHITE : DARKPURPLE);
            }

            const int pixel = 2;
            const int espaco = 4;
            const int passo = 8 * pixel + espaco;
            const int larguraTotal = ATIRADOR_SAUDE * passo - espaco;

            float centroX = at->corpo.x + at->corpo.width / 2.0f;
            int inicioX = (int)(centroX - larguraTotal / 2.0f);
            int yCoracoes = (int)topoAtirador - 7 * pixel - 3;

            for (int i = 0; i < ATIRADOR_SAUDE; i++) {
                DesenharCoracao(inicioX + i * passo, yCoracoes, pixel, i < at->saude);
            }
            break;
        }
        }
    }
}