#include "raylib.h"
#include "ranking.h"

// Provisorio: tabela simples. A tela final de resultados e da Pessoa 4.
void DesenharRanking(const Ranking *ranking, int x, int y)
{
    DrawText("RANKING - MENORES TEMPOS", x, y, 28, GOLD);
    y += 44;

    if (ranking->quantidade == 0) {
        DrawText("Nenhum tempo registrado ainda.", x, y, 22, LIGHTGRAY);
        return;
    }

    char tempo[16];
    for (int i = 0; i < ranking->quantidade; i++) {
        const Resultado *r = &ranking->resultados[i];
        Color cor = (i == 0) ? GOLD : RAYWHITE;     // 1o lugar em dourado
        FormatarTempo(r->tempo, tempo, sizeof tempo);
        DrawText(TextFormat("%2d.", i + 1), x, y, 22, cor);
        DrawText(r->apelido, x + 50, y, 22, cor);
        DrawText(tempo, x + 260, y, 22, cor);
        y += 30;
    }
}
