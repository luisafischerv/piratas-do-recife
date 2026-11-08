#include "raylib.h"
#include "inimigos_ia.h"
#include "marujo.h"

static void DesenharMarujo(const Inimigo *m)
{
    Color cor = MAROON;
    if (m->estado == ESTADO_PREPARACAO) cor = ORANGE;
    DrawRectangleRec(m->corpo, cor);
    float olhoX = (m->direcao > 0) ? m->corpo.x + m->corpo.width - 12
                                   : m->corpo.x + 4;
    DrawRectangle((int)olhoX, (int)(m->corpo.y + 12), 8, 8, RAYWHITE);
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