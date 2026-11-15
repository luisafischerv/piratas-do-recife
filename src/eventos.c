#include "eventos.h"
#include "marujo.h"

void InicializarSequencia(SequenciaEventos *seq)
{
    seq->quantidade = 0;
    seq->proximo = 0;
    seq->relogio = 0.0f;
}

bool CadastrarEvento(SequenciaEventos *seq, Evento evento)
{
    if (seq->quantidade >= MAX_EVENTOS) return false;

    evento.ordemCadastro = seq->quantidade;
    evento.executado = false;
    seq->eventos[seq->quantidade] = evento;
    seq->quantidade++;
    return true;
}

void OrdenarEventos(Evento v[], int n)
{
    for (int i = 1; i < n; i++) {
        Evento chave = v[i];
        int j = i - 1;

        while (j >= 0 && v[j].instante > chave.instante) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = chave;
    }
}

static void ExecutarUmEvento(Evento *e, ListaInimigos *lista,
                             bool disparoCanhao[], int maxCanhoes)
{
    switch (e->tipo) {
    case EVENTO_SURGIR_MARUJO:
        InserirInimigo(lista, CriarMarujo(e->x, e->chaoY,
                                          e->limiteEsquerdo, e->limiteDireito));
        break;
    case EVENTO_DISPARO_CANHAO:
        // So marca; o canhao le essa marca quando existir.
        if (disparoCanhao != NULL && e->indiceCanhao >= 0 && e->indiceCanhao < maxCanhoes)
            disparoCanhao[e->indiceCanhao] = true;
        break;
    }
    e->executado = true;
}

int ExecutarEventos(SequenciaEventos *seq, float dt, ListaInimigos *lista,
                    bool disparoCanhao[], int maxCanhoes)
{
    seq->relogio += dt;
    int executados = 0;

    while (seq->proximo < seq->quantidade &&
           seq->eventos[seq->proximo].instante <= seq->relogio) {
        ExecutarUmEvento(&seq->eventos[seq->proximo], lista, disparoCanhao, maxCanhoes);
        seq->proximo++;
        executados++;
    }
    return executados;
}

void ReiniciarEventos(SequenciaEventos *seq)
{
    seq->relogio = 0.0f;
    seq->proximo = 0;
    for (int i = 0; i < seq->quantidade; i++) {
        seq->eventos[i].executado = false;
    }
}