#include <stdio.h>
#include "combate.h"
#include "inimigos_ia.h"
#include "marujo.h"

static int falhas = 0;

// Se a condicao for falsa, avisa qual linha falhou.
#define CONFERIR(cond) do { \
    if (!(cond)) { printf("  FALHOU (linha %d): %s\n", __LINE__, #cond); falhas++; } \
} while (0)

#define DT (1.0f / 60.0f)
#define CHAO 620.0f

// Nico "de mentira": so o corpo e a direcao importam para a espada.
static Jogador NicoEm(float x, int direcao)
{
    Jogador j = {0};
    j.corpo = (Rectangle){ x, CHAO - 80.0f, 56.0f, 80.0f };
    j.direcao = direcao;
    return j;
}

// Marujo parado no lugar (limites iguais ao proprio corpo).
static Inimigo MarujoEm(float x)
{
    return CriarMarujo(x, CHAO, x, x + 44.0f);
}

int main(void)
{
    Espada espada;
    ListaInimigos lista;

    printf("1) Clique gera golpe; segurar nao gera outro\n");
    InicializarEspada(&espada);
    CONFERIR(AtualizarAtaqueEspada(&espada, true, DT) == true);
    CONFERIR(espada.ativa && espada.idGolpe == 1);
    for (int i = 0; i < 60; i++) AtualizarAtaqueEspada(&espada, false, DT); // 1 s sem clicar
    CONFERIR(!espada.ativa && espada.idGolpe == 1);

    printf("2) Clique durante a recarga e ignorado\n");
    InicializarEspada(&espada);
    AtualizarAtaqueEspada(&espada, true, DT);
    CONFERIR(AtualizarAtaqueEspada(&espada, true, DT) == false);  // 1 quadro depois
    CONFERIR(espada.idGolpe == 1);
    for (int i = 0; i < 20; i++) AtualizarAtaqueEspada(&espada, false, DT); // passa a recarga
    CONFERIR(AtualizarAtaqueEspada(&espada, true, DT) == true);
    CONFERIR(espada.idGolpe == 2);

    printf("3) Dois inimigos no alcance: 1 acerto cada, mesmo com varios quadros\n");
    InicializarLista(&lista);
    Jogador nico = NicoEm(100.0f, 1);               // corpo vai de 100 a 156
    NoInimigo *a = InserirInimigo(&lista, MarujoEm(160.0f));
    NoInimigo *b = InserirInimigo(&lista, MarujoEm(190.0f));
    InicializarEspada(&espada);
    AtualizarAtaqueEspada(&espada, true, DT);
    int total = 0;
    for (int i = 0; i < 8; i++) {                    // area ativa por ~0,13 s
        total += VerificarAcertosEspada(&espada, &nico, &lista);
        AtualizarAtaqueEspada(&espada, false, DT);
    }
    CONFERIR(total == 2);
    CONFERIR(a->dados.saude == 2 && b->dados.saude == 2);

    printf("4) Golpe novo acerta de novo\n");
    for (int i = 0; i < 20; i++) AtualizarAtaqueEspada(&espada, false, DT);
    AtualizarAtaqueEspada(&espada, true, DT);
    CONFERIR(VerificarAcertosEspada(&espada, &nico, &lista) == 2);
    CONFERIR(a->dados.saude == 1 && b->dados.saude == 1);

    printf("5) Inimigo longe nao e atingido\n");
    NoInimigo *longe = InserirInimigo(&lista, MarujoEm(600.0f));
    CONFERIR(VerificarAcertosEspada(&espada, &nico, &lista) == 0); // mesmo golpe, a e b ja levaram
    CONFERIR(longe->dados.saude == 3);
    LiberarInimigos(&lista);

    printf("6) Virado para a esquerda nao acerta quem esta a direita\n");
    InicializarLista(&lista);
    nico = NicoEm(100.0f, -1);
    NoInimigo *direita = InserirInimigo(&lista, MarujoEm(160.0f));
    NoInimigo *esquerda = InserirInimigo(&lista, MarujoEm(40.0f));
    InicializarEspada(&espada);
    AtualizarAtaqueEspada(&espada, true, DT);
    CONFERIR(VerificarAcertosEspada(&espada, &nico, &lista) == 1);
    CONFERIR(direita->dados.saude == 3 && esquerda->dados.saude == 2);

    printf("7) Espada inativa nao causa dano\n");
    for (int i = 0; i < 20; i++) AtualizarAtaqueEspada(&espada, false, DT);
    CONFERIR(!espada.ativa);
    CONFERIR(VerificarAcertosEspada(&espada, &nico, &lista) == 0);
    LiberarInimigos(&lista);

    printf("8) Inimigo com saude 0 sai da lista na AtualizarInimigos\n");
    InicializarLista(&lista);
    nico = NicoEm(100.0f, 1);
    InserirInimigo(&lista, MarujoEm(160.0f));
    InicializarEspada(&espada);
    for (int golpe = 0; golpe < 3; golpe++) {        // 3 golpes = saude do marujo
        for (int i = 0; i < 20; i++) AtualizarAtaqueEspada(&espada, false, DT);
        AtualizarAtaqueEspada(&espada, true, DT);
        VerificarAcertosEspada(&espada, &nico, &lista);
    }
    CONFERIR(lista.inicio->dados.saude == 0);
    CONFERIR(lista.quantidade == 1);                 // ainda esta na lista...
    AtualizarInimigos(&lista, &nico, DT);
    CONFERIR(lista.quantidade == 0 && lista.inicio == NULL && lista.fim == NULL); // ...agora saiu
    LiberarInimigos(&lista);

    if (falhas == 0) printf("\nTODOS OS TESTES PASSARAM\n");
    else printf("\n%d FALHA(S)\n", falhas);
    return falhas == 0 ? 0 : 1;
}
