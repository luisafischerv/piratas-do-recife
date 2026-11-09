#include <stdio.h>
#include "inimigos.h"

static int falhas = 0;

// Se a condicao for falsa, avisa qual linha falhou.
#define CONFERIR(cond) do { \
    if (!(cond)) { printf("  FALHOU (linha %d): %s\n", __LINE__, #cond); falhas++; } \
} while (0)

// Cria um inimigo usando a saude como "etiqueta" para identificar cada no.
static Inimigo Etiqueta(int numero)
{
    Inimigo i = {0};
    i.saude = numero;
    return i;
}

// Mostra a lista nos dois sentidos, ex.: "ida: 1 2 3 | volta: 3 2 1 | qtd=3"
static void Mostrar(const ListaInimigos *l)
{
    printf("  ida:");
    for (NoInimigo *no = l->inicio; no != NULL; no = no->proximo) printf(" %d", no->dados.saude);
    printf(" | volta:");
    for (NoInimigo *no = l->fim; no != NULL; no = no->anterior) printf(" %d", no->dados.saude);
    printf(" | qtd=%d\n", l->quantidade);
}

int main(void)
{
    ListaInimigos l;

    printf("1) Lista vazia\n");
    InicializarLista(&l);
    Mostrar(&l);
    CONFERIR(l.inicio == NULL && l.fim == NULL && l.quantidade == 0);

    printf("2) Inserir e remover um unico no\n");
    NoInimigo *unico = InserirInimigo(&l, Etiqueta(1));
    Mostrar(&l);
    CONFERIR(l.inicio == unico && l.fim == unico);
    CONFERIR(unico->anterior == NULL && unico->proximo == NULL);
    RemoverInimigo(&l, unico);
    Mostrar(&l);
    CONFERIR(l.inicio == NULL && l.fim == NULL && l.quantidade == 0);

    printf("3) Inserir 1, 2, 3\n");
    InserirInimigo(&l, Etiqueta(1));
    InserirInimigo(&l, Etiqueta(2));
    InserirInimigo(&l, Etiqueta(3));
    Mostrar(&l);
    CONFERIR(l.quantidade == 3);
    CONFERIR(l.inicio->dados.saude == 1 && l.fim->dados.saude == 3);
    CONFERIR(l.inicio->proximo->anterior == l.inicio);   // ida e volta batem

    printf("4) Remover do meio (o 2)\n");
    RemoverInimigo(&l, l.inicio->proximo);
    Mostrar(&l);
    CONFERIR(l.quantidade == 2);
    CONFERIR(l.inicio->proximo == l.fim && l.fim->anterior == l.inicio);

    printf("5) Remover o primeiro (o 1)\n");
    RemoverInimigo(&l, l.inicio);
    Mostrar(&l);
    CONFERIR(l.inicio == l.fim && l.inicio->dados.saude == 3);
    CONFERIR(l.inicio->anterior == NULL);

    printf("6) Remover o ultimo (o 3, que agora e o unico)\n");
    RemoverInimigo(&l, l.fim);
    Mostrar(&l);
    CONFERIR(l.inicio == NULL && l.fim == NULL && l.quantidade == 0);

    printf("7) Liberar tudo e reutilizar a lista\n");
    for (int i = 1; i <= 5; i++) InserirInimigo(&l, Etiqueta(i));
    Mostrar(&l);
    LiberarInimigos(&l);
    Mostrar(&l);
    CONFERIR(l.inicio == NULL && l.fim == NULL && l.quantidade == 0);
    InserirInimigo(&l, Etiqueta(9));
    Mostrar(&l);
    CONFERIR(l.quantidade == 1);
    LiberarInimigos(&l);

    if (falhas == 0) printf("\nOK: todos os testes da lista passaram.\n");
    else printf("\n%d verificacao(oes) falharam.\n", falhas);
    return falhas;
}