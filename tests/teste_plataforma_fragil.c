#include <assert.h>
#include <stdio.h>
#include "fase.h"

int main(void)
{
    Fase f = {0};
    Jogador j = {0};
    f.qtdPlataformas = 1;
    f.plataformas[0] = (Plataforma){.area = {100, 200, 160, 20},
                                    .tipo = PLAT_FRAGIL, .estado = PLAT_NORMAL,
                                    .ativa = true};
    j.corpo = (Rectangle){120, 120, 56, 80};
    j.estaNoChao = true;
    AtualizarPlataformasFrageis(&f, &j, 0.016f);
    assert(f.plataformas[0].estado == PLAT_AVISO && f.plataformas[0].ativa);
    j.estaNoChao = false;
    AtualizarPlataformasFrageis(&f, &j, 0.3f);
    assert(f.plataformas[0].ativa);
    AtualizarPlataformasFrageis(&f, &j, 0.31f);
    assert(!f.plataformas[0].ativa && f.plataformas[0].estado == PLAT_CAIDA);
    puts("Plataforma fragil passou nos testes.");
    return 0;
}
