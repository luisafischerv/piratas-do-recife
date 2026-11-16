#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "cipo.h"

// Integra a mesma gravidade e velocidade horizontal do jogador sem janela/teclado.
static void TestarTravessia(float dt)
{
    SistemaCipos entrada = {0};
    Jogador entrando = {0};
    ReiniciarCipos(&entrada);
    entrando.corpo = (Rectangle){1600, 540, 56, 80};
    entrando.velocidadeY = -520;
    for (int k = 0; k < (int)(1.5f / dt) && entrada.agarrado < 0; k++) {
        entrando.corpo.x += 260 * dt;
        entrando.velocidadeY += 1300 * dt;
        entrando.corpo.y += entrando.velocidadeY * dt;
        AtualizarCipos(&entrada, &entrando, dt, 1, true, false);
    }
    assert(entrada.agarrado == 0);
    for (int origem = 0; origem < 3; origem++) {
        int sucessos = 0;
        for (int espera = 1; espera < (int)(3.0f / dt); espera++) {
            SistemaCipos s = {0};
            Jogador j = {0};
            ReiniciarCipos(&s);
            j.corpo = (Rectangle){0, 0, 56, 80};
            s.agarrado = origem;
            s.cipos[origem].velocidadeAngular = 1.2f;
            for (int k = 0; k < espera; k++)
                AtualizarCipos(&s, &j, dt, 1, false, false);
            AtualizarCipos(&s, &j, dt, 1, false, true);
            // Tentar ignorar todos os proximos cipos nao alcanca a margem final.
            SistemaCipos semAgarrar = s;
            Jogador atalho = j;
            if (origem < 2) {
                while (atalho.corpo.y <= 820) {
                    float base = atalho.corpo.y + 80;
                    atalho.corpo.x += 260 * dt;
                    atalho.velocidadeY += 1300 * dt;
                    atalho.corpo.y += atalho.velocidadeY * dt;
                    AtualizarCipos(&semAgarrar, &atalho, dt, 1, false, false);
                    assert(!(base <= 620 && atalho.corpo.y + 80 >= 620 &&
                             atalho.velocidadeY >= 0 && atalho.corpo.x + 56 > 2960));
                }
            }
            for (int k = 0; k < (int)(2.0f / dt); k++) {
                float baseAnterior = j.corpo.y + j.corpo.height;
                j.corpo.x += 260.0f * dt;
                j.velocidadeY += 1300.0f * dt;
                j.corpo.y += j.velocidadeY * dt;
                AtualizarCipos(&s, &j, dt, 1, true, false);
                if (s.agarrado >= 0) {
                    // Nao pode saltar um cipo inteiro.
                    assert(s.agarrado <= origem + 1);
                    if (s.agarrado == origem + 1) sucessos++;
                    break;
                }
                if (baseAnterior <= 620 && j.corpo.y + 80 >= 620 &&
                    j.velocidadeY >= 0 && j.corpo.x + 56 > 2960) {
                    assert(origem == 2);
                    sucessos++;
                    break;
                }
                if (j.corpo.y > 820) break;
            }
        }
        printf("Cipo %d, dt %.4f: %d momentos de soltura viaveis\n", origem + 1, dt, sucessos);
        assert(sucessos >= 3);
    }
}

int main(void)
{
    SistemaCipos s = {0};
    Jogador j = {0};
    ReiniciarCipos(&s);
    j.corpo = (Rectangle){1782, 535, 56, 80};
    j.estaNoChao = true;
    AtualizarCipos(&s, &j, 0.016f, 1, true, false);
    assert(s.agarrado == -1);
    j.estaNoChao = false;
    AtualizarCipos(&s, &j, 0.016f, 1, true, false);
    assert(s.agarrado == 0);
    for (int i = 0; i < 30; i++) AtualizarCipos(&s, &j, 0.016f, 1, false, false);
    assert(s.cipos[0].angulo > 0 && s.cipos[0].angulo <= 0.9f);
    AtualizarCipos(&s, &j, 0.016f, 1, false, true);
    assert(s.agarrado == -1 && j.velocidadeY < 0 && s.bloqueio > 0);
    AtualizarCipos(&s, &j, 0.016f, 0, true, false);
    assert(s.agarrado == -1);
    ReiniciarCipos(&s);
    assert(s.agarrado == -1 && fabsf(s.cipos[0].angulo) < 0.001f);
    TestarTravessia(1.0f / 30.0f);
    TestarTravessia(1.0f / 60.0f);
    TestarTravessia(1.0f / 120.0f);
    puts("Testes de cipo passaram.");
    return 0;
}
