#include <assert.h>
#include <stdio.h>
#include "combate.h"
#include "inimigos_ia.h"

int main(void)
{
    Jogador nico = {0};
    nico.corpo = (Rectangle){100, 100, 56, 80};
    nico.direcao = 1;
    Rectangle area = AreaEspada(&nico);
    assert(RetangulosSobrepostos(area, (Rectangle){135, 110, 10, 60}));
    assert(!RetangulosSobrepostos(area, (Rectangle){80, 110, 15, 60}));
    assert(!RetangulosSobrepostos(area, (Rectangle){230, 110, 20, 60}));
    nico.direcao = -1;
    area = AreaEspada(&nico);
    assert(RetangulosSobrepostos(area, (Rectangle){105, 110, 10, 60}));
    assert(!RetangulosSobrepostos(area, (Rectangle){160, 110, 15, 60}));
    puts("Area da espada passou nos testes.");
    return 0;
}
