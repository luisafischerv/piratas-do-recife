#include <stdio.h>
#include "eventos.h"

static int falhas = 0;

#define CONFERIR(cond) do { \
    if (!(cond)) { printf("  FALHOU (linha %d): %s\n", __LINE__, #cond); falhas++; } \
} while (0)

static Evento Marujo(float instante, float x)
{
    Evento e = {0};
    e.instante = instante;
    e.tipo = EVENTO_SURGIR_MARUJO;
    e.x = x;
    e.chaoY = 620.0f;
    e.limiteEsquerdo = x - 100.0f;
    e.limiteDireito = x + 100.0f;
    return e;
}

static Evento Canhao(float instante, int indice)
{
    Evento e = {0};
    e.instante = instante;
    e.tipo = EVENTO_DISPARO_CANHAO;
    e.indiceCanhao = indice;
    return e;
}

static void Mostrar(const SequenciaEventos *seq)
{
    printf("  ");
    for (int i = 0; i < seq->quantidade; i++)
        printf("[t=%.1f cad=%d] ", seq->eventos[i].instante, seq->eventos[i].ordemCadastro);
    printf("\n");
}

int main(void)
{
    static SequenciaEventos seq;
    ListaInimigos lista;
    InicializarLista(&lista);
    bool canhoes[3] = { false, false, false };

    printf("1) Cadastro fora de ordem, com empates\n");
    InicializarSequencia(&seq);
    CadastrarEvento(&seq, Canhao(5.0f, 0));
    CadastrarEvento(&seq, Marujo(2.0f, 300));
    CadastrarEvento(&seq, Canhao(2.0f, 1));
    CadastrarEvento(&seq, Marujo(0.5f, 600));
    CadastrarEvento(&seq, Canhao(2.0f, 2));
    CadastrarEvento(&seq, Marujo(8.0f, 900));
    Mostrar(&seq);

    printf("2) Depois do Insertion Sort\n");
    OrdenarEventos(seq.eventos, seq.quantidade);
    Mostrar(&seq);
    CONFERIR(seq.eventos[0].instante == 0.5f);
    CONFERIR(seq.eventos[1].ordemCadastro == 1);
    CONFERIR(seq.eventos[2].ordemCadastro == 2);
    CONFERIR(seq.eventos[3].ordemCadastro == 4);
    CONFERIR(seq.eventos[4].instante == 5.0f);
    CONFERIR(seq.eventos[5].instante == 8.0f);

    printf("3) Executa no tempo certo\n");
    CONFERIR(ExecutarEventos(&seq, 0.4f, &lista, canhoes, 3) == 0);
    CONFERIR(ExecutarEventos(&seq, 0.2f, &lista, canhoes, 3) == 1);
    CONFERIR(lista.quantidade == 1);

    printf("4) Varios eventos no mesmo quadro\n");
    CONFERIR(ExecutarEventos(&seq, 2.0f, &lista, canhoes, 3) == 3);
    CONFERIR(lista.quantidade == 2);
    CONFERIR(canhoes[1] && canhoes[2] && !canhoes[0]);

    printf("5) Nao repete\n");
    CONFERIR(ExecutarEventos(&seq, 0.1f, &lista, canhoes, 3) == 0);
    ExecutarEventos(&seq, 10.0f, &lista, canhoes, 3);
    CONFERIR(seq.proximo == seq.quantidade);
    CONFERIR(lista.quantidade == 3);
    CONFERIR(ExecutarEventos(&seq, 10.0f, &lista, canhoes, 3) == 0);

    printf("6) Reinicio: tudo de novo, sem duplicar\n");
    LiberarInimigos(&lista);
    ReiniciarEventos(&seq);
    CONFERIR(seq.proximo == 0 && seq.relogio == 0.0f);
    for (int quadro = 0; quadro < 60 * 10; quadro++)
        ExecutarEventos(&seq, 1.0f / 60.0f, &lista, NULL, 0);
    CONFERIR(lista.quantidade == 3);

    printf("7) Vetor cheio recusa\n");
    InicializarSequencia(&seq);
    for (int i = 0; i < MAX_EVENTOS; i++) CONFERIR(CadastrarEvento(&seq, Canhao((float)i, 0)));
    CONFERIR(!CadastrarEvento(&seq, Canhao(99.0f, 0)));

    LiberarInimigos(&lista);
    if (falhas == 0) printf("\nOK: todos os testes de eventos passaram.\n");
    else printf("\n%d verificacao(oes) falharam.\n", falhas);
    return falhas;
}