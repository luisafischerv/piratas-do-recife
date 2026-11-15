#include "cipo.h"
#include <math.h>

static Vector2 Ponta(const Cipo *c)
{
    return (Vector2){c->pivo.x + sinf(c->angulo) * c->comprimento,
                     c->pivo.y + cosf(c->angulo) * c->comprimento};
}

void ReiniciarCipos(SistemaCipos *s)
{
    s->cipos[0] = (Cipo){{1810, 295}, 240, 0, 0};
    s->cipos[1] = (Cipo){{2230, 265}, 240, 0, 0};
    s->cipos[2] = (Cipo){{2650, 295}, 240, 0, 0};
    // Cipo de aprendizado: subida sobre as raizes e soltura na passarela fragil.
    s->cipos[3] = (Cipo){{460, 180}, 300, 0, 0};
    s->agarrado = -1;
    s->bloqueio = 0;
    s->impulsoX = 0;
}

void CarregarCipos(SistemaCipos *s)
{
    *s = (SistemaCipos){0};
    ReiniciarCipos(s);
    s->textura = LoadTexture("assets/images/cenarios/fase2/cipo_proposta.png");
    s->nico = LoadTexture("assets/images/nico/nico_cipo_proposta.png");
    if (s->textura.id) SetTextureFilter(s->textura, TEXTURE_FILTER_BILINEAR);
    if (s->nico.id) SetTextureFilter(s->nico, TEXTURE_FILTER_BILINEAR);
}

void DescarregarCipos(SistemaCipos *s)
{
    if (s->textura.id) UnloadTexture(s->textura);
    if (s->nico.id) UnloadTexture(s->nico);
    s->textura.id = s->nico.id = 0;
}

void AtualizarCipos(SistemaCipos *s, Jogador *j, float dt, float direcao, bool agarrar, bool soltar)
{
    if (s->bloqueio > 0) s->bloqueio -= dt;
    if (s->agarrado < 0 && agarrar && s->bloqueio <= 0 && !j->estaNoChao) {
        for (int i = 0; i < QUANTIDADE_CIPOS; i++) {
            Vector2 p = Ponta(&s->cipos[i]);
            float dx = j->corpo.x + j->corpo.width / 2 - p.x;
            float dy = j->corpo.y - p.y;
            if (dx * dx + dy * dy < 85.0f * 85.0f) {
                s->agarrado = i;
                s->cipos[i].velocidadeAngular = direcao * 1.2f;
                s->impulsoX = 0;
                break;
            }
        }
    }
    if (s->agarrado < 0) {
        if (s->impulsoX != 0) {
            j->corpo.x += s->impulsoX * dt;
            s->impulsoX *= fmaxf(0.0f, 1.0f - 2.0f * dt);
            if (j->estaNoChao || fabsf(s->impulsoX) < 1.0f) s->impulsoX = 0;
        }
        return;
    }
    Cipo *c = &s->cipos[s->agarrado];
    c->velocidadeAngular += (-5.4f * sinf(c->angulo) + direcao * 3.8f) * dt;
    c->velocidadeAngular *= fmaxf(0.0f, 1.0f - 0.35f * dt);
    c->angulo += c->velocidadeAngular * dt;
    if (c->angulo > 0.9f) { c->angulo = 0.9f; c->velocidadeAngular *= -0.3f; }
    if (c->angulo < -0.9f) { c->angulo = -0.9f; c->velocidadeAngular *= -0.3f; }
    Vector2 p = Ponta(c);
    j->corpo.x = p.x - j->corpo.width / 2;
    j->corpo.y = p.y + 8;
    j->velocidadeY = 0;
    j->estaNoChao = false;
    j->preparoSalto = 0;
    if (soltar) {
        s->impulsoX = fmaxf(-280, fminf(280, c->velocidadeAngular * c->comprimento * cosf(c->angulo)));
        // A subida depende do momento do balanco: soltar parado nao da um superpulo.
        float subida = c->velocidadeAngular * c->comprimento * sinf(c->angulo);
        j->velocidadeY = -fmaxf(120.0f, fminf(380.0f, subida));
        j->saltoDiagonal = true;
        j->tempoNoAr = 0.0f;
        s->agarrado = -1;
        s->bloqueio = 0.35f;
    }
}

void DesenharCipos(const SistemaCipos *s)
{
    for (int i = 0; i < QUANTIDADE_CIPOS; i++) {
        const Cipo *c = &s->cipos[i];
        if (s->textura.id) {
            // A arte tem margens transparentes; recorta a fibra e ancora no topo.
            Rectangle origem = {350, 25, 190, 1725};
            Rectangle destino = {c->pivo.x, c->pivo.y, 32, c->comprimento};
            DrawTexturePro(s->textura, origem, destino, (Vector2){16, 0},
                           -c->angulo * RAD2DEG, WHITE);
        } else {
            DrawLineEx(c->pivo, Ponta(c), 5, DARKGREEN);
        }
    }
}

void DesenharNicoNoCipo(const SistemaCipos *s, const Jogador *j)
{
    if (s->agarrado < 0 || !s->nico.id) { DesenharJogador(j); return; }
    const Cipo *c = &s->cipos[s->agarrado];
    Vector2 p = Ponta(c);
    int quadro = fabsf(c->velocidadeAngular) < 0.4f ? 0 :
                 fabsf(c->angulo) > 0.55f ? 3 : 2;
    float w = (float)s->nico.width / 4, h = (float)s->nico.height / 2;
    Rectangle origem = {(quadro % 4) * w, (quadro / 4) * h, w, h};
    float altura = 150, largura = altura * w / h;
    // As maos ficam proximas do centro superior de cada quadro.
    Rectangle destino = {p.x - largura * 0.55f, p.y - altura * 0.04f, largura, altura};
    DrawTexturePro(s->nico, origem, destino, (Vector2){0, 0}, 0, WHITE);
}
