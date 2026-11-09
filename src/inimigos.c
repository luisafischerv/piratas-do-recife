#include "inimigos.h"
#include <stdlib.h>

void InicializarLista(ListaInimigos *lista)
{
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->quantidade = 0;
}

NoInimigo *InserirInimigo(ListaInimigos *lista, Inimigo dados)
{
    NoInimigo *novo = malloc(sizeof(NoInimigo));
    if (novo == NULL) return NULL;

    novo->dados = dados;
    novo->proximo = NULL;
    novo->anterior = lista->fim;

    if (lista->fim == NULL) {
        lista->inicio = novo;
    } else {
        lista->fim->proximo = novo;
    }
    lista->fim = novo;
    lista->quantidade++;
    return novo;
}

void RemoverInimigo(ListaInimigos *lista, NoInimigo *no)
{
    if (no == NULL) return;

    if (no->anterior != NULL) {
        no->anterior->proximo = no->proximo;
    } else {
        lista->inicio = no->proximo;
    }

    if (no->proximo != NULL) {
        no->proximo->anterior = no->anterior;
    } else {
        lista->fim = no->anterior;
    }

    free(no);
    lista->quantidade--;
}

void LiberarInimigos(ListaInimigos *lista)
{
    NoInimigo *atual = lista->inicio;
    while (atual != NULL) {
        NoInimigo *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    InicializarLista(lista); // para as refencias inicio e fim ficarem NULL
}