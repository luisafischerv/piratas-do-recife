#include <assert.h>
#include <stdio.h>
#include "canhoes_escalada.h"

int main(void)
{
    CanhoesEscalada s;
    SistemaTiros tiros;
    ListaInimigos inimigos;
    Jogador j = {0};
    j.corpo = (Rectangle){612, 1000, 56, 80};
    InicializarLista(&inimigos);
    InicializarTiros(&tiros);
    ReiniciarCanhoesEscalada(&s);
    for (int i = 0; i < CANHOES_ESCALADA_LATERAIS; i++) assert(s.laterais[i].velocidadeTiro == 720.0f);
    bool viuSegundoLateral = false;
    bool viuTerceiroLateral = false;
    bool viuAvisoLateral = false, viuTiroLateral = false, viuAvisoVertical = false, viuDano = false, repetiu = false;
    for (int k = 0; k < 1600; k++) {
        int dano = AtualizarCanhoesEscalada(&s, &tiros, &inimigos, &j, 1.0f / 60);
        if (s.laterais[3].estado == CANHAO_CARREGANDO) viuAvisoLateral = true;
        if (s.laterais[4].estado == CANHAO_CARREGANDO) viuSegundoLateral = true;
        if (s.laterais[5].estado == CANHAO_CARREGANDO) viuTerceiroLateral = true;
        for (int i = 0; i < MAX_TIROS; i++) if (tiros.tiros[i].ativo) {
            viuTiroLateral = true;
            assert(tiros.tiros[i].velocidade == 720.0f || tiros.tiros[i].velocidade == -720.0f);
        }
        if (s.avisoVertical[1] > 0) viuAvisoVertical = true;
        if (dano > 0) viuDano = true;
        if (k > 600 && s.avisoVertical[1] > 0) repetiu = true;
    }
    assert(viuAvisoLateral && viuTiroLateral && viuAvisoVertical && viuDano && repetiu);
    assert(viuSegundoLateral && viuTerceiroLateral && s.rodada >= 2);
    for (int trecho = 0; trecho < 4; trecho++) {
        ReiniciarCanhoesEscalada(&s);
        InicializarTiros(&tiros);
        j.corpo.y = 1400 - trecho * 330;
        AtualizarCanhoesEscalada(&s, &tiros, &inimigos, &j, 0.01f);
        float primeiroRitmo = s.eventos.eventos[1].instante;
        bool acionou[3] = {false};
        for (int k = 0; k < 635; k++) {
            AtualizarCanhoesEscalada(&s, &tiros, &inimigos, &j, 1.0f / 60);
            for (int i = 0; i < 3; i++)
                if (s.laterais[trecho * 3 + i].estado == CANHAO_CARREGANDO) acionou[i] = true;
        }
        assert(acionou[0] && acionou[1] && acionou[2]);
        assert(s.rodada == 1 && s.eventos.eventos[1].instante != primeiroRitmo);
        for (int i = 1; i < s.eventos.quantidade; i++)
            assert(s.eventos.eventos[i].instante > s.eventos.eventos[i - 1].instante);
    }
    j.corpo.y = 180;
    assert(AtualizarCanhoesEscalada(&s, &tiros, &inimigos, &j, 0.016f) == 0);
    assert(s.terminou && !s.balaAtiva[1]);
    for (int i = 0; i < MAX_TIROS; i++) assert(!tiros.tiros[i].ativo);
    ReiniciarCanhoesEscalada(&s);
    assert(s.eventos.relogio == 0 && !s.balaAtiva[1] && s.avisoVertical[1] == 0);
    puts("Canhoes da escalada passaram.");
    return 0;
}
