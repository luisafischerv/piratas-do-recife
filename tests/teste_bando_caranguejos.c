#include <assert.h>
#include <stdio.h>
#include "bando_caranguejos.h"

int main(void) {
    BandoCaranguejos b;
    Jogador j = {0};
    j.corpo = (Rectangle){600, 520, 56, 80};
    ReiniciarBando(&b);
    assert(!b.ativo && b.direcao == -1);
    int dano = 0;
    for (int i = 0; i < 800 && b.direcao == -1; i++) dano += AtualizarBando(&b, &j, 1, 1.0f / 60);
    assert(dano > 0 && !b.ativo && b.direcao == 1);
    j.corpo.y = 400;
    dano = 0;
    for (int i = 0; i < 800 && b.direcao == 1; i++) dano += AtualizarBando(&b, &j, 1, 1.0f / 60);
    assert(dano == 0 && !b.ativo && b.direcao == -1);
    ReiniciarBando(&b);
    assert(!b.ativo && b.espera == 5.0f);
    for (int estagio = 1; estagio <= 3; estagio++) {
        ReiniciarBando(&b);
        AtualizarBando(&b, &j, estagio, 5.0f);
        assert(b.ativo && b.quantidade == estagio * 2);
    }
    puts("Bandos de caranguejos passaram.");
    return 0;
}
