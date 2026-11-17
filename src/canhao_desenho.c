#include "raylib.h"
#include "canhao.h"

// Provisorio: canhao em retangulos. O aviso pisca e mostra a linha do tiro.
void DesenharCanhoes(const Canhao canhoes[], int quantidade)
{
    for (int i = 0; i < quantidade; i++) {
        const Canhao *c = &canhoes[i];
        Rectangle r = c->corpo;

        // Base e cano (o cano fica do lado para onde ele atira).
        DrawRectangleRec(r, (Color){ 55, 55, 60, 255 });
        float canoX = (c->direcao > 0) ? r.x + r.width : r.x - 22.0f;
        DrawRectangle((int)canoX, (int)(r.y + r.height * 0.2f), 22, 14, (Color){ 35, 35, 40, 255 });
        DrawRectangleLinesEx(r, 2.0f, BLACK);

        if (c->estado == CANHAO_CARREGANDO) {
            // Pisca cada vez mais rapido perto do tiro.
            bool aceso = ((int)(GetTime() * (c->timer < 0.4f ? 16.0 : 8.0)) % 2) == 0;
            if (aceso) DrawRectangleRec(r, Fade(ORANGE, 0.75f));
            // Linha na altura do tiro, mostrando o caminho da bala.
            float y = r.y + r.height * ALTURA_TIRO_RELATIVA + 4.0f;
            float x0 = (c->direcao > 0) ? r.x + r.width : r.x - 700.0f;
            DrawRectangle((int)x0, (int)y - 1, 700, 3, Fade(ORANGE, 0.5f));
            DrawText("!", (int)(r.x + r.width / 2 - 5), (int)(r.y - 34), 30, ORANGE);
        }
    }
}
