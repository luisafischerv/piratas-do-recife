#include "raylib.h"
#include "inimigos_ia.h"
#include "marujo.h"

static Texture2D texturaMarujo;
static Texture2D texturaAndar;
static Texture2D texturaAtaque;

void CarregarVisualInimigos(void)
{
    texturaMarujo = LoadTexture("assets/images/marujo/marujo_proposta.png");
    texturaAndar = LoadTexture("assets/images/marujo/marujo_andar.png");
    texturaAtaque = LoadTexture("assets/images/marujo/marujo_ataque.png");
    if (texturaMarujo.id) SetTextureFilter(texturaMarujo, TEXTURE_FILTER_BILINEAR);
    if (texturaAndar.id) SetTextureFilter(texturaAndar, TEXTURE_FILTER_BILINEAR);
    if (texturaAtaque.id) SetTextureFilter(texturaAtaque, TEXTURE_FILTER_BILINEAR);
}

void DescarregarVisualInimigos(void)
{
    if (texturaMarujo.id) UnloadTexture(texturaMarujo);
    if (texturaAndar.id) UnloadTexture(texturaAndar);
    if (texturaAtaque.id) UnloadTexture(texturaAtaque);
    texturaMarujo.id = 0;
    texturaAndar.id = texturaAtaque.id = 0;
}

static void DesenharMarujo(const Inimigo *m)
{
    if (texturaMarujo.id) {
        int quadro = 0;
        Texture2D textura = texturaMarujo;
        if ((m->estado == ESTADO_PATRULHA || m->estado == ESTADO_APROXIMACAO) && texturaAndar.id) {
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
        DrawTexturePro(textura, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
    } else {
        DrawRectangleRec(m->corpo, MAROON);
    }
    if (m->estado == ESTADO_PREPARACAO) {
        DrawRectangleLinesEx(AreaGolpeMarujo(m), 2.0f, ORANGE);
    } else if (m->estado == ESTADO_ATAQUE) {
        DrawRectangleRec(AreaGolpeMarujo(m), Fade(RED, 0.7f));
    }
    for (int i = 0; i < m->saude; i++)
        DrawRectangle((int)(m->corpo.x + i * 10), (int)(m->corpo.y - 10), 8, 5, RED);
}

void DesenharInimigos(const ListaInimigos *lista)
{
    for (const NoInimigo *no = lista->inicio; no != NULL; no = no->proximo) {
        switch (no->dados.tipo) {
        case INIMIGO_MARUJO:
            DesenharMarujo(&no->dados);
            break;
        case INIMIGO_ATIRADOR:
        case INIMIGO_CARANGUEJO:
            DrawRectangleRec(no->dados.corpo, BROWN); // provisorio (só até Pessoa 1 e 2 mudarem)
            break;
        }
    }
}
