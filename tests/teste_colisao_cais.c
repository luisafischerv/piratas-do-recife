#include <assert.h>
#include <stdio.h>
#include "colisao.h"

int main(void)
{
    Fase f = {0};
    Jogador j = {0};
    f.qtdPlataformas = 1;
    f.plataformas[0].ativa = true;
    f.plataformas[0].tipo = PLAT_FIXA;
    f.plataformas[0].area = (Rectangle){100, 100, 200, 20};

    // Subida atravessa a passarela.
    Rectangle anterior = {150, 115, 40, 60};
    j.corpo = (Rectangle){150, 90, 40, 60};
    j.velocidadeY = -100;
    ResolverColisoesCenario(&j, &f, anterior);
    assert(j.corpo.y == 90 && j.velocidadeY == -100 && !j.estaNoChao);

    // Descida pousa na superficie superior.
    anterior = (Rectangle){150, 35, 40, 60};
    j.corpo = (Rectangle){150, 45, 40, 60};
    j.velocidadeY = 100;
    ResolverColisoesCenario(&j, &f, anterior);
    assert(j.corpo.y == 40 && j.velocidadeY == 0 && j.estaNoChao);

    // Caixa ainda bloqueia o movimento lateral.
    f.plataformas[0].tipo = PLAT_CAIXA;
    anterior = (Rectangle){55, 100, 40, 60};
    j.corpo = (Rectangle){65, 100, 40, 60};
    j.velocidadeY = 0;
    ResolverColisoesCenario(&j, &f, anterior);
    assert(j.corpo.x == 60);
    puts("Colisoes do cais passaram.");
    return 0;
}
