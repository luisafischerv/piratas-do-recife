#include <stdio.h>
#include "canhao.h"
#include "eventos.h"

static int falhas = 0;

#define CONFERIR(cond) do { \
    if (!(cond)) { printf("  FALHOU (linha %d): %s\n", __LINE__, #cond); falhas++; } \
} while (0)

#define DT (1.0f / 60.0f)

static int TirosAtivos(const SistemaTiros *s)
{
    int n = 0;
    for (int i = 0; i < MAX_TIROS; i++)
        if (s->tiros[i].ativo) n++;
    return n;
}

// Avanca 'segundos' e devolve quantos tiros sairam nesse tempo.
static int Rodar(Canhao c[], int n, bool pedidos[], SistemaTiros *t, float segundos)
{
    int total = 0;
    int quadros = (int)(segundos * 60.0f + 0.5f);
    for (int i = 0; i < quadros; i++) total += AtualizarCanhoes(c, n, pedidos, t, DT);
    return total;
}

int main(void)
{
    SistemaTiros tiros;
    Canhao c[2];
    bool pedidos[2];

    printf("1) Sem pedido, o canhao nao atira\n");
    InicializarTiros(&tiros);
    c[0] = CriarCanhao(1000.0f, 620.0f, -1);
    pedidos[0] = false;
    CONFERIR(Rodar(c, 1, pedidos, &tiros, 3.0f) == 0);
    CONFERIR(c[0].estado == CANHAO_PARADO);

    printf("2) Pedido: aviso primeiro, tiro so depois do aviso\n");
    pedidos[0] = true;
    AtualizarCanhoes(c, 1, pedidos, &tiros, DT);
    CONFERIR(c[0].estado == CANHAO_CARREGANDO);
    CONFERIR(pedidos[0] == false);                     // marca foi atendida e limpa
    CONFERIR(Rodar(c, 1, pedidos, &tiros, 0.9f) == 0); // ainda avisando
    CONFERIR(Rodar(c, 1, pedidos, &tiros, 0.2f) == 1); // agora atira
    CONFERIR(TirosAtivos(&tiros) == 1);
    CONFERIR(c[0].estado == CANHAO_INTERVALO);

    printf("3) O tiro sai na direcao do canhao e fere o Nico (nao e do jogador)\n");
    const Tiro *t = NULL;
    for (int i = 0; i < MAX_TIROS; i++) if (tiros.tiros[i].ativo) t = &tiros.tiros[i];
    CONFERIR(t != NULL && t->velocidade < 0.0f);
    CONFERIR(t != NULL && !t->doJogador);
    CONFERIR(t != NULL && t->area.x < c[0].corpo.x);

    printf("4) Depois do intervalo, volta a ficar parado\n");
    Rodar(c, 1, pedidos, &tiros, 1.0f);
    CONFERIR(c[0].estado == CANHAO_PARADO);

    printf("5) Pedido com o canhao ocupado espera e nao se perde\n");
    InicializarTiros(&tiros);
    c[0] = CriarCanhao(1000.0f, 620.0f, -1);
    pedidos[0] = true;
    AtualizarCanhoes(c, 1, pedidos, &tiros, DT);      // comeca o aviso
    pedidos[0] = true;                                // novo pedido durante o aviso
    int disparos = Rodar(c, 1, pedidos, &tiros, 5.0f);
    CONFERIR(disparos == 2);                          // os dois pedidos viraram tiros
    CONFERIR(pedidos[0] == false);

    printf("6) Reinicio: para tudo e apaga pedidos pendentes\n");
    c[0] = CriarCanhao(1000.0f, 620.0f, -1);
    c[1] = CriarCanhao(100.0f, 620.0f, 1);
    pedidos[0] = pedidos[1] = true;
    AtualizarCanhoes(c, 2, pedidos, &tiros, DT);
    pedidos[1] = true;
    ReiniciarCanhoes(c, 2, pedidos);
    CONFERIR(c[0].estado == CANHAO_PARADO && c[1].estado == CANHAO_PARADO);
    CONFERIR(!pedidos[0] && !pedidos[1]);
    InicializarTiros(&tiros);
    CONFERIR(Rodar(c, 2, pedidos, &tiros, 3.0f) == 0);

    printf("7) Caminho completo: evento -> marca -> aviso -> tiro\n");
    static SequenciaEventos seq;
    ListaInimigos lista;
    InicializarLista(&lista);
    InicializarTiros(&tiros);
    c[0] = CriarCanhao(1000.0f, 620.0f, -1);
    c[1] = CriarCanhao(100.0f, 620.0f, 1);
    pedidos[0] = pedidos[1] = false;
    InicializarSequencia(&seq);
    CadastrarDisparoCanhao(&seq, 2.0f, 1);
    CadastrarDisparoCanhao(&seq, 1.0f, 0);
    CadastrarDisparoCanhao(&seq, 1.0f, 7);            // indice invalido: ignorado
    OrdenarEventos(seq.eventos, seq.quantidade);
    ReiniciarEventos(&seq);
    int tirosTotal = 0;
    for (int q = 0; q < 60 * 4; q++) {                // 4 segundos
        ExecutarEventos(&seq, DT, &lista, pedidos, 2);
        tirosTotal += AtualizarCanhoes(c, 2, pedidos, &tiros, DT);
    }
    CONFERIR(seq.proximo == seq.quantidade);
    CONFERIR(tirosTotal == 2);                        // um tiro de cada canhao

    printf("8) Fase sem eventos (cais) fica com a sequencia vazia; ilha tem eventos ordenados\n");
    MontarEventosDaFase(&seq, 1);
    CONFERIR(seq.quantidade == 0);
    CONFERIR(ExecutarEventos(&seq, 100.0f, &lista, NULL, 0) == 0);
    MontarEventosDaFase(&seq, 2);
    CONFERIR(seq.quantidade > 0 && seq.proximo == 0 && seq.relogio == 0.0f);
    for (int i = 1; i < seq.quantidade; i++)
        CONFERIR(seq.eventos[i - 1].instante <= seq.eventos[i].instante);

    LiberarInimigos(&lista);
    if (falhas == 0) printf("\nOK: todos os testes do canhao passaram.\n");
    else printf("\n%d verificacao(oes) falharam.\n", falhas);
    return falhas;
}
