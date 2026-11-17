#include <assert.h>
#include <stdio.h>
#include "cordas.h"

int main(void)
{
    SistemaCordas s = {0};
    Jogador j = {0};
    ReiniciarCordas(&s);
    j.corpo = (Rectangle){612, 1720, 56, 80};
    AtualizarCordas(&s, &j, 0.016f, -1, 0, true, false);
    assert(s.agarrada == 1 && j.corpo.y < 1720);
    AtualizarCordas(&s, &j, 0.016f, 0, 1, true, true);
    assert(s.agarrada == -1 && j.velocidadeY < 0 && s.bloqueio > 0);
    // Mesmo movimento e gravidade do jogador, segurando D e E no ar.
    for (int k = 0; k < 60 && s.agarrada < 0; k++) {
        j.corpo.x += 260.0f / 60.0f;
        j.velocidadeY += 1300.0f / 60.0f;
        j.corpo.y += j.velocidadeY / 60.0f;
        AtualizarCordas(&s, &j, 1.0f / 60.0f, 0, 1, true, false);
    }
    assert(s.agarrada == 2);
    SoltarCordaPorDano(&s, &j);
    assert(s.agarrada == -1 && j.velocidadeY > 0 && !j.estaNoChao);
    // Segurar E nao cancela a queda imediatamente.
    for (int k = 0; k < 20; k++) {
        AtualizarCordas(&s, &j, 0.016f, 0, 0, true, false);
        assert(s.agarrada == -1);
    }
    float bloqueioAntes = s.bloqueio;
    SoltarCordaPorDano(&s, &j);
    assert(s.bloqueio == bloqueioAntes);
    for (int k = 0; k < 20; k++)
        AtualizarCordas(&s, &j, 0.016f, 0, 0, true, false);
    assert(s.agarrada == 2);
    ReiniciarCordas(&s);
    assert(s.agarrada == -1 && s.bloqueio == 0);
    puts("Escalada e troca de cordas passaram.");
    return 0;
}
