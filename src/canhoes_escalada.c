#include "canhoes_escalada.h"

static const float faixas[] = {460, 640, 820};

static void PrepararTrecho(CanhoesEscalada *s, int trecho)
{
    s->trecho = trecho;
    InicializarSequencia(&s->eventos);
    // Ritmos alternados entre rodadas e alturas, com pausas de duracao variavel.
    static const float tempos[2][6] = {
        {0.55f, 1.65f, 2.55f, 3.85f, 4.80f, 6.05f},
        {0.40f, 1.30f, 2.65f, 3.60f, 4.85f, 5.85f}
    };
    static const int ordem[2][6] = {{0, 1, 2, 0, 1, 2}, {2, 1, 0, 2, 1, 0}};
    int ritmo = (s->rodada + trecho) % 2;
    for (int i = 0; i < 6; i++)
        CadastrarDisparoCanhao(&s->eventos, tempos[ritmo][i], trecho * 3 + ordem[ritmo][i]);
    CadastrarDisparoCanhao(&s->eventos, 0.2f, CANHOES_ESCALADA_LATERAIS + (trecho + s->rodada + 1) % 3);
    CadastrarDisparoCanhao(&s->eventos, 2.35f, CANHOES_ESCALADA_LATERAIS + (trecho + s->rodada) % 3);
    CadastrarDisparoCanhao(&s->eventos, 4.65f, CANHOES_ESCALADA_LATERAIS + (trecho + s->rodada + 2) % 3);
    OrdenarEventos(s->eventos.eventos, s->eventos.quantidade);
}

void ReiniciarCanhoesEscalada(CanhoesEscalada *s)
{
    *s = (CanhoesEscalada){0};
    for (int trecho = 0; trecho < 4; trecho++) {
        float altura = 1450.0f - trecho * 330.0f;
        int direcao = trecho % 2 == 0 ? 1 : -1;
        for (int i = 0; i < 3; i++) {
            int lado = i % 2 == 0 ? direcao : -direcao;
            Canhao *c = &s->laterais[trecho * 3 + i];
            *c = CriarCanhao(lado > 0 ? 240 : 980, altura - i * 100.0f, lado);
            c->velocidadeTiro = 720.0f;
        }
    }
    PrepararTrecho(s, 0);
}

int AtualizarCanhoesEscalada(CanhoesEscalada *s, SistemaTiros *tiros, ListaInimigos *inimigos, const Jogador *j, float dt)
{
    if (j->corpo.y <= 200 || s->terminou) {
        // Chegada segura: encerra a bateria e remove seus disparos restantes.
        if (!s->terminou) {
            ReiniciarCanhoes(s->laterais, CANHOES_ESCALADA_LATERAIS, s->pedidos);
            for (int i = 0; i < 3; i++) { s->avisoVertical[i] = 0; s->balaAtiva[i] = false; s->pedidos[i + CANHOES_ESCALADA_LATERAIS] = false; }
            for (int i = 0; i < MAX_TIROS; i++) if (!tiros->tiros[i].doJogador) tiros->tiros[i].ativo = false;
            s->terminou = true;
        }
        return 0;
    }
    int trecho = j->corpo.y > 1250 ? 0 : j->corpo.y > 920 ? 1 : j->corpo.y > 590 ? 2 : 3;
    if (trecho != s->trecho) {
        ReiniciarCanhoes(s->laterais, CANHOES_ESCALADA_LATERAIS, s->pedidos);
        PrepararTrecho(s, trecho);
    }
    ExecutarEventos(&s->eventos, dt, inimigos, s->pedidos, CANHOES_ESCALADA_TOTAL);
    AtualizarCanhoes(s->laterais, CANHOES_ESCALADA_LATERAIS, s->pedidos, tiros, dt);
    int dano = 0;
    for (int i = 0; i < 3; i++) {
        if (s->pedidos[i + CANHOES_ESCALADA_LATERAIS]) {
            s->pedidos[i + CANHOES_ESCALADA_LATERAIS] = false;
            s->avisoVertical[i] = 1.2f;
        }
        if (s->avisoVertical[i] > 0) {
            s->avisoVertical[i] -= dt;
            if (s->avisoVertical[i] <= 0) {
                s->yBala[i] = 1800;
                s->balaAtiva[i] = true;
            }
        }
        if (s->balaAtiva[i]) {
            float anterior = s->yBala[i];
            s->yBala[i] -= 850.0f * dt;
            Rectangle varredura = {faixas[i] - 12, s->yBala[i], 24, anterior - s->yBala[i] + 24};
            Rectangle alvo = {j->corpo.x, j->corpo.y - 30, j->corpo.width, j->corpo.height + 30};
            if (CheckCollisionRecs(varredura, alvo)) { dano++; s->balaAtiva[i] = false; }
            if (s->yBala[i] < 0) s->balaAtiva[i] = false;
        }
    }
    if (s->eventos.relogio >= 7.6f) {
        s->rodada++;
        PrepararTrecho(s, trecho);
    }
    return dano;
}

void DesenharCanhoesEscalada(const CanhoesEscalada *s)
{
    DesenharCanhoes(s->laterais, CANHOES_ESCALADA_LATERAIS);
    for (int i = 0; i < 3; i++) {
        DrawRectangle((int)faixas[i] - 20, 1800, 40, 35, DARKGRAY);
        DrawRectangle((int)faixas[i] - 10, 1780, 20, 25, GRAY);
        if (s->avisoVertical[i] > 0) {
            DrawRectangle((int)faixas[i] - 3, 180, 6, 1600, Fade(ORANGE, 0.55f));
            DrawCircle((int)faixas[i], 1780, 14, ORANGE);
        }
        if (s->balaAtiva[i]) DrawCircle((int)faixas[i], (int)s->yBala[i], 12, ORANGE);
    }
}
