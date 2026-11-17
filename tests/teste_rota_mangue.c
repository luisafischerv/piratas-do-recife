#include <assert.h>
#include <stdio.h>
#include "cipo.h"
#include "colisao.h"

static void Passo(Jogador *j, SistemaCipos *s, const Fase *f, float dt, bool agarrar)
{
    Rectangle antes = j->corpo;
    j->corpo.x += 260 * dt;
    j->velocidadeY += 1300 * dt;
    j->corpo.y += j->velocidadeY * dt;
    AtualizarCipos(s, j, dt, 1, agarrar, false);
    if (s->agarrado < 0) ResolverColisoesCenario(j, f, antes);
}

static void TestarRota(const Fase *f, float dt)
{
    SistemaCipos s = {0};
    Jogador j = {0};
    ReiniciarCipos(&s);
    j.corpo = (Rectangle){290, 540, 56, 80};
    j.velocidadeY = -520;
    for (int k = 0; k < 100 && s.agarrado < 0; k++) Passo(&j, &s, f, dt, true);
    assert(s.agarrado == 3);

    int pousos = 0;
    for (int espera = 1; espera < (int)(2 / dt); espera++) {
        ReiniciarCipos(&s);
        s.agarrado = 3;
        s.cipos[3].velocidadeAngular = 1.2f;
        j = (Jogador){0};
        j.corpo = (Rectangle){0, 0, 56, 80};
        bool atingiuRaiz = false;
        for (int k = 0; k < espera; k++) {
            AtualizarCipos(&s, &j, dt, 1, false, false);
            for (int p = 0; p < f->qtdPlataformas; p++)
                if (f->plataformas[p].tipo == PLAT_RAIZ && CheckCollisionRecs(j.corpo, f->plataformas[p].area))
                    atingiuRaiz = true;
            if (atingiuRaiz) break;
        }
        if (atingiuRaiz) continue;
        AtualizarCipos(&s, &j, dt, 1, false, true);
        for (int k = 0; k < (int)(2 / dt); k++) {
            Passo(&j, &s, f, dt, false);
            if (j.estaNoChao) {
                if (j.corpo.y == 540 && j.corpo.x + 56 > 855 && j.corpo.x < 955) pousos++;
                break;
            }
            if (j.corpo.y > 820) break;
        }
    }
    printf("dt %.4f: %d solturas alcancam a plataforma diretamente\n", dt, pousos);
    assert(pousos > 2);

    // Cada salto do parkour tem uma solucao com a fisica normal, sem impulso de cipo.
    float partidas[] = {910, 1110, 1270};
    float alturas[] = {620, 620, 600};
    float destinos[] = {1040, 1220, 1400};
    for (int n = 0; n < 3; n++) {
        ReiniciarCipos(&s);
        j = (Jogador){0};
        j.corpo = (Rectangle){partidas[n], alturas[n] - 80, 56, 80};
        j.velocidadeY = -520;
        bool chegou = false;
        for (int k = 0; k < (int)(1.5f / dt); k++) {
            Passo(&j, &s, f, dt, false);
            if (j.estaNoChao && j.corpo.x + 56 > destinos[n]) { chegou = true; break; }
        }
        assert(chegou);
    }
}

int main(void)
{
    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_WINDOW_HIDDEN);
    InitWindow(1280, 720, "Validacao da rota");
    Fase f = {0};
    assert(CarregarFase(&f, 2));
    TestarRota(&f, 1.0f / 30);
    TestarRota(&f, 1.0f / 60);
    TestarRota(&f, 1.0f / 120);
    RenderTexture2D preview = LoadRenderTexture(1280, 720);
    BeginTextureMode(preview);
    ClearBackground(BLACK);
    DesenharFase(&f);
    SistemaCipos s = {0};
    CarregarCipos(&s);
    DesenharCipos(&s);
    EndTextureMode();
    Image imagem = LoadImageFromTexture(preview.texture);
    ImageFlipVertical(&imagem);
    ExportImage(imagem, "build/rota_mangue.png");
    UnloadImage(imagem);
    UnloadRenderTexture(preview);
    DescarregarCipos(&s);
    DescarregarFase(&f);
    CloseWindow();
    puts("Rota do mangue validada.");
    return 0;
}
