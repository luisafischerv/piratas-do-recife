#include "raylib.h"
#include "combate.h"

// Provisorio: area dourada enquanto o golpe esta ativo.
// A animacao do Nico com a espada (nico_espada.png) e separada do colisor.
void DesenharEspada(const Espada *espada, const Jogador *jogador)
{
    if (!espada->ativa) return;
    Rectangle area = AreaEspada(jogador);
    DrawRectangleRec(area, Fade(GOLD, 0.45f));
    DrawRectangleLinesEx(area, 2.0f, GOLD);
}
