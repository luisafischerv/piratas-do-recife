#include "cordas.h"
#include <math.h>

static const float posicoes[] = {460, 640, 820};
#define CORDA_TOPO 180.0f
#define CORDA_BASE 1800.0f
#define VELOCIDADE_ESCALADA 170.0f

void ReiniciarCordas(SistemaCordas *s) { s->agarrada = -1; s->bloqueio = 0; s->tempoAnimacao = 0; }
void CarregarCordas(SistemaCordas *s)
{
    *s = (SistemaCordas){0};
    ReiniciarCordas(s);
    s->textura = LoadTexture("assets/images/cenarios/fase3/corda_escalada_proposta.png");
    s->nico = LoadTexture("assets/images/nico/nico_escalada_sem_corda_proposta.png");
    if (s->textura.id) SetTextureFilter(s->textura, TEXTURE_FILTER_BILINEAR);
    if (s->nico.id) SetTextureFilter(s->nico, TEXTURE_FILTER_BILINEAR);
}
void DescarregarCordas(SistemaCordas *s)
{
    if (s->textura.id) UnloadTexture(s->textura);
    if (s->nico.id) UnloadTexture(s->nico);
    s->textura.id = s->nico.id = 0;
}
void AtualizarCordas(SistemaCordas *s, Jogador *j, float dt, int vertical, int horizontal, bool agarrar, bool saltar)
{
    if (s->bloqueio > 0) s->bloqueio -= dt;
    if (s->agarrada < 0 && agarrar && s->bloqueio <= 0) {
        float centro = j->corpo.x + j->corpo.width / 2;
        float melhor = 42;
        for (int i = 0; i < NUM_CORDAS_ESCALADA; i++) {
            float distancia = fabsf(centro - posicoes[i]);
            if (distancia < melhor && j->corpo.y + j->corpo.height > CORDA_TOPO && j->corpo.y < CORDA_BASE) {
                melhor = distancia;
                s->agarrada = i;
            }
        }
    }
    if (s->agarrada < 0) return;
    j->corpo.x = posicoes[s->agarrada] - j->corpo.width / 2;
    float yAnterior = j->corpo.y;
    j->corpo.y += vertical * VELOCIDADE_ESCALADA * dt;
    if (j->corpo.y < CORDA_TOPO) j->corpo.y = CORDA_TOPO;
    if (j->corpo.y + j->corpo.height > CORDA_BASE) j->corpo.y = CORDA_BASE - j->corpo.height;
    if (j->corpo.y != yAnterior) {
        s->tempoAnimacao += (vertical < 0 ? 1 : -1) * dt * 7;
        while (s->tempoAnimacao < 0) s->tempoAnimacao += 8;
        while (s->tempoAnimacao >= 8) s->tempoAnimacao -= 8;
    }
    j->velocidadeY = 0;
    j->estaNoChao = false;
    j->preparoSalto = 0;
    if (saltar) {
        s->agarrada = -1;
        s->bloqueio = 0.25f;
        j->velocidadeY = -520;
        j->saltoDiagonal = horizontal != 0;
        j->tempoNoAr = 0;
        if (horizontal) j->direcao = horizontal;
    }
}

void DesenharNicoNaCorda(const SistemaCordas *s, const Jogador *j)
{
    if (s->agarrada < 0 || !s->nico.id) { DesenharJogador(j); return; }
    int quadro = (int)s->tempoAnimacao;
    float largura = (float)s->nico.width / 4;
    float altura = (float)s->nico.height / 2;
    Rectangle origem = {(quadro % 4) * largura, (quadro / 4) * altura, largura, altura};
    float alturaVisual = 150, larguraVisual = alturaVisual * largura / altura;
    // As maos ficam na mesma linha da corda, sem alterar o colisor do jogador.
    Rectangle destino = {posicoes[s->agarrada] - larguraVisual * 0.64f,
                        j->corpo.y + j->corpo.height - alturaVisual * 0.94f,
                        larguraVisual, alturaVisual};
    DrawTexturePro(s->nico, origem, destino, (Vector2){0, 0}, 0, WHITE);
}
void DesenharCordas(const SistemaCordas *s)
{
    for (int i = 0; i < NUM_CORDAS_ESCALADA; i++) {
        if (s->textura.id) {
            Rectangle origem = {300, 0, 125, (float)s->textura.height};
            Rectangle destino = {posicoes[i] - 14, CORDA_TOPO, 28, CORDA_BASE - CORDA_TOPO};
            DrawTexturePro(s->textura, origem, destino, (Vector2){0, 0}, 0, WHITE);
        } else DrawLineEx((Vector2){posicoes[i], CORDA_TOPO}, (Vector2){posicoes[i], CORDA_BASE}, 8, BROWN);
    }
}
