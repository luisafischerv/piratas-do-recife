#include <stdio.h>
#include "marujo.h"
#include "tiros.h"

static int falhas = 0;

#define CONFERIR(cond) do { \
    if (!(cond)) { printf("  FALHOU (linha %d): %s\n", __LINE__, #cond); falhas++; } \
} while (0)

// Arena de teste: de x=100 a x=1100, chao em 620.
#define CHAO 620.0f
#define ARENA_E 100.0f
#define ARENA_D 1100.0f

static bool DentroDaArena(const Inimigo *m)
{
    return m->corpo.x >= ARENA_E && m->corpo.x + m->corpo.width <= ARENA_D;
}

int main(void)
{
    ListaInimigos lista;

    printf("1) Arena vazia: convoca a quantidade pedida\n");
    InicializarLista(&lista);
    CONFERIR(ConvocarMarujos(&lista, 2, 3, CHAO, ARENA_E, ARENA_D, 300.0f) == 2);
    CONFERIR(lista.quantidade == 2 && ContarMarujos(&lista) == 2);

    printf("2) Nunca passa do limite de marujos vivos\n");
    CONFERIR(ConvocarMarujos(&lista, 5, 3, CHAO, ARENA_E, ARENA_D, 300.0f) == 1);
    CONFERIR(ContarMarujos(&lista) == 3);
    CONFERIR(ConvocarMarujos(&lista, 2, 3, CHAO, ARENA_E, ARENA_D, 300.0f) == 0);
    CONFERIR(lista.quantidade == 3);

    printf("3) Marujo morto libera vaga\n");
    lista.inicio->dados.saude = 0;                 // derrotado (ainda nao removido)
    CONFERIR(ContarMarujos(&lista) == 2);
    CONFERIR(ConvocarMarujos(&lista, 2, 3, CHAO, ARENA_E, ARENA_D, 300.0f) == 1);
    LiberarInimigos(&lista);

    printf("4) Outros inimigos nao contam para o limite de marujos\n");
    InicializarLista(&lista);
    InserirInimigo(&lista, CriarAtirador(500.0f, CHAO));
    CONFERIR(ConvocarMarujos(&lista, 2, 2, CHAO, ARENA_E, ARENA_D, 300.0f) == 2);
    CONFERIR(lista.quantidade == 3 && ContarMarujos(&lista) == 2);
    LiberarInimigos(&lista);

    printf("5) Surgem do lado oposto ao Nico, dentro da arena e virados para ele\n");
    InicializarLista(&lista);
    ConvocarMarujos(&lista, 3, 3, CHAO, ARENA_E, ARENA_D, 200.0f);      // Nico a esquerda
    for (NoInimigo *no = lista.inicio; no != NULL; no = no->proximo) {
        CONFERIR(DentroDaArena(&no->dados));
        CONFERIR(no->dados.corpo.x > 600.0f);                           // lado direito
        CONFERIR(no->dados.direcao == -1);
        CONFERIR(no->dados.corpo.y + no->dados.corpo.height == CHAO);   // pes no chao
    }
    LiberarInimigos(&lista);
    InicializarLista(&lista);
    ConvocarMarujos(&lista, 3, 3, CHAO, ARENA_E, ARENA_D, 1000.0f);     // Nico a direita
    for (NoInimigo *no = lista.inicio; no != NULL; no = no->proximo) {
        CONFERIR(DentroDaArena(&no->dados));
        CONFERIR(no->dados.corpo.x < 600.0f);                           // lado esquerdo
        CONFERIR(no->dados.direcao == 1);
    }

    printf("6) Nao surgem um em cima do outro\n");
    NoInimigo *a = lista.inicio, *b = a->proximo;
    CONFERIR(b->dados.corpo.x - a->dados.corpo.x >= a->dados.corpo.width);

    printf("7) Derrota no chefe: LiberarInimigos limpa os convocados\n");
    LiberarInimigos(&lista);
    CONFERIR(lista.quantidade == 0 && lista.inicio == NULL && lista.fim == NULL);
    CONFERIR(ConvocarMarujos(&lista, 2, 2, CHAO, ARENA_E, ARENA_D, 300.0f) == 2); // de novo, sem acumular
    CONFERIR(lista.quantidade == 2);

    printf("8) Quantidade zero ou negativa nao faz nada\n");
    CONFERIR(ConvocarMarujos(&lista, 0, 5, CHAO, ARENA_E, ARENA_D, 300.0f) == 0);
    CONFERIR(ConvocarMarujos(&lista, -2, 5, CHAO, ARENA_E, ARENA_D, 300.0f) == 0);
    CONFERIR(lista.quantidade == 2);

    LiberarInimigos(&lista);
    if (falhas == 0) printf("\nOK: todos os testes da convocacao passaram.\n");
    else printf("\n%d verificacao(oes) falharam.\n", falhas);
    return falhas;
}
