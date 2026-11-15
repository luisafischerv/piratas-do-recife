#include "interface.h"
#include "raylib.h"

void DesenharCoracao(int x, int y, int pixel, bool cheio)
{
    static const char desenho[7][9] = {
        ".##..##.",
        "#+R##RR#",
        "#RRRRRR#",
        "#RRRRRR#",
        ".#RRRR#.",
        "..#RR#..",
        "...##..."
    };

    for (int linha = 0; linha < 7; linha++) {
        for (int coluna = 0; coluna < 8; coluna++) {
            char parte = desenho[linha][coluna];

            if (parte == '.' || (!cheio && parte != '#')) {
                continue;
            }

            Color cor = BLACK;
            if (parte == 'R') cor = (Color){ 205, 24, 52, 255 };
            if (parte == '+') cor = (Color){ 255, 150, 165, 255 };

            DrawRectangle(x + coluna * pixel, y + linha * pixel,
                          pixel, pixel, cor);
        }
    }
}