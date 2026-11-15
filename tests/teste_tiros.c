#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "tiros.h"

static int Contar(const SistemaTiros *s)
{
    int n = 0;
    for (int i = 0; i < MAX_TIROS; i++) n += s->tiros[i].ativo;
    return n;
}

int main(void)
{
    SistemaTiros s;
    ListaInimigos lista;
    Jogador j = {0};
    Fase fase = {0};
    fase.largura = 1000;
    j.corpo = (Rectangle){100, 100, 56, 80};
    j.direcao = 1;
    InicializarLista(&lista);
    InicializarTiros(&s);
    AtualizarPistola(&s, &j, true, 0.01f);
    assert(fabsf(s.tiros[0].area.y - (j.corpo.y + j.corpo.height * ALTURA_TIRO_RELATIVA)) < 0.01f);
    AtualizarPistola(&s, &j, true, 0.1f);
    assert(Contar(&s) == 1);
    AtualizarPistola(&s, &j, true, 0.3f);
    assert(Contar(&s) == 2);

    InicializarTiros(&s);
    NoInimigo *alvo = InserirInimigo(&lista, CriarAtirador(240, 180));
    fase.qtdPlataformas = 1;
    fase.plataformas[0].ativa = true;
    fase.plataformas[0].area = (Rectangle){200, 90, 5, 100};
    AtualizarPistola(&s, &j, true, 0.01f);
    AtualizarTiros(&s, &fase, &lista, &j, 1.0f);
    assert(alvo->dados.saude == 3 && Contar(&s) == 0);
    fase.qtdPlataformas = 0;
    InicializarTiros(&s);
    AtualizarPistola(&s, &j, true, 0.01f);
    AtualizarTiros(&s, &fase, &lista, &j, 1.0f);
    assert(alvo->dados.saude == 2 && Contar(&s) == 0);

    InicializarTiros(&s);
    alvo->dados.timerEstado = 0.0f;
    AtualizarAtiradores(&s, &lista, &j, 0.01f);
    assert(alvo->dados.estado == ESTADO_PREPARACAO && Contar(&s) == 0);
    AtualizarAtiradores(&s, &lista, &j, 0.79f);
    assert(Contar(&s) == 0);
    AtualizarAtiradores(&s, &lista, &j, 0.02f);
    assert(Contar(&s) == 1);
    assert(AtualizarTiros(&s, &fase, &lista, &j, 1.0f) == 1);
    float xAposDisparo = alvo->dados.corpo.x;
    AtualizarAtiradores(&s, &lista, &j, 0.8f);
    assert(alvo->dados.corpo.x < xAposDisparo);
    LiberarInimigos(&lista);
    puts("Testes de tiros passaram.");
    return 0;
}
