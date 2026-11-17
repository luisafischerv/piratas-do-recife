#include <stdio.h>
#include "inimigos_ia.h"
#include "caranguejo.h"
#include "combate.h"

static int falhas = 0;

#define CONFERIR(cond) do { \
    if (!(cond)) { printf("  FALHOU (linha %d): %s\n", __LINE__, #cond); falhas++; } \
} while (0)

#define DT (1.0f / 60.0f)
#define CHAO 620.0f

static Jogador NicoEm(float x)
{
    Jogador j = {0};
    j.corpo = (Rectangle){ x, CHAO - 80.0f, 56.0f, 80.0f };
    j.direcao = 1;
    return j;
}

int main(void)
{
    ListaInimigos lista;

    printf("1) Longe do Nico: so patrulha, sem dano\n");
    InicializarLista(&lista);
    InserirInimigo(&lista, CriarCaranguejo(1000.0f, CHAO, 800.0f, 1200.0f));
    Jogador nico = NicoEm(100.0f);
    int dano = 0;
    for (int i = 0; i < 120; i++) dano += AtualizarInimigos(&lista, &nico, DT);
    CONFERIR(dano == 0);
    CONFERIR(lista.inicio->dados.estado == ESTADO_PATRULHA);
    LiberarInimigos(&lista);

    printf("2) Perto: prepara (aviso), investe e acerta 1 vez so\n");
    InicializarLista(&lista);
    NoInimigo *c = InserirInimigo(&lista, CriarCaranguejo(500.0f, CHAO, 100.0f, 900.0f));
    nico = NicoEm(350.0f);                       // a esquerda, dentro da percepcao
    AtualizarInimigos(&lista, &nico, DT);
    CONFERIR(c->dados.estado == ESTADO_PREPARACAO);
    dano = 0;
    for (int i = 0; i < 30; i++) dano += AtualizarInimigos(&lista, &nico, DT); // 0,5 s: ainda avisando
    CONFERIR(dano == 0);
    for (int i = 0; i < 60; i++) dano += AtualizarInimigos(&lista, &nico, DT); // investida
    CONFERIR(dano == 1);                         // atravessa o Nico mas so 1 de dano

    printf("3) Morre com 2 golpes de espada e sai da lista\n");
    Espada espada;
    InicializarEspada(&espada);
    nico = NicoEm(c->dados.corpo.x - 60.0f);     // colado, virado para ele
    for (int golpe = 0; golpe < CARANGUEJO_SAUDE; golpe++) {
        for (int i = 0; i < 45; i++) AtualizarAtaqueEspada(&espada, false, DT);
        AtualizarAtaqueEspada(&espada, true, DT);
        VerificarAcertosEspada(&espada, &nico, &lista);
    }
    CONFERIR(c->dados.saude == 0);
    AtualizarInimigos(&lista, &nico, DT);
    CONFERIR(lista.quantidade == 0 && lista.inicio == NULL);

    LiberarInimigos(&lista);
    if (falhas == 0) printf("\nOK: todos os testes do caranguejo passaram.\n");
    else printf("\n%d verificacao(oes) falharam.\n", falhas);
    return falhas;
}
