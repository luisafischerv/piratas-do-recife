#include <stdio.h>
#include "caranguejo.h"
#include "inimigos_ia.h"

static int falhas = 0;
#define CONFERIR(cond) do { \
    if (!(cond)) { printf("  FALHOU (linha %d): %s\n", __LINE__, #cond); falhas++; } \
} while (0)

#define DT (1.0f / 60.0f)

static Jogador NicoEm(float x, float y)
{
    Jogador j = {0};
    j.corpo = (Rectangle){ x, y, 56.0f, 80.0f };
    return j;
}

// Avanca o caranguejo ate mudar de estado (ou ate o limite de quadros).
static int AvancarAteMudar(Inimigo *c, const Jogador *j, int *dano, int limite)
{
    EstadoInimigo inicial = c->estado;
    int quadros = 0;
    while (c->estado == inicial && quadros < limite) {
        *dano += AtualizarCaranguejo(c, j, DT);
        quadros++;
    }
    return quadros;
}

int main(void)
{
    int dano = 0;

    printf("1) Patrulha sem o Nico por perto e respeita os limites\n");
    Inimigo c = CriarCaranguejo(3000.0f, 620.0f, 2970.0f, 3270.0f);
    Jogador longe = NicoEm(100.0f, 540.0f);
    for (int i = 0; i < 600; i++) {
        dano += AtualizarCaranguejo(&c, &longe, DT);
        CONFERIR(c.corpo.x >= 2970.0f && c.corpo.x + c.corpo.width <= 3270.0f);
    }
    CONFERIR(c.estado == ESTADO_PATRULHA && dano == 0);

    printf("2) Nico no alto (cipo) nao e percebido\n");
    c = CriarCaranguejo(3000.0f, 620.0f, 2970.0f, 3270.0f);
    Jogador alto = NicoEm(3150.0f, 300.0f);
    for (int i = 0; i < 60; i++) AtualizarCaranguejo(&c, &alto, DT);
    CONFERIR(c.estado == ESTADO_PATRULHA);

    printf("3) Percebe, prepara parado e mira no Nico\n");
    c = CriarCaranguejo(3000.0f, 620.0f, 2970.0f, 3270.0f);
    Jogador nico = NicoEm(3200.0f, 540.0f);
    AtualizarCaranguejo(&c, &nico, DT);
    CONFERIR(c.estado == ESTADO_PREPARACAO && c.direcao == 1);
    float xParado = c.corpo.x;
    int quadros = AvancarAteMudar(&c, &nico, &dano, 200);
    CONFERIR(c.corpo.x == xParado);
    CONFERIR(quadros >= 35 && quadros <= 37);
    CONFERIR(c.estado == ESTADO_ATAQUE);

    printf("4) Investida acerta uma unica vez\n");
    dano = 0;
    AvancarAteMudar(&c, &nico, &dano, 200);
    CONFERIR(dano == 1);
    CONFERIR(c.estado == ESTADO_RECUPERACAO);
    CONFERIR(c.corpo.x > xParado);

    printf("5) Recupera e volta a patrulhar\n");
    AvancarAteMudar(&c, &nico, &dano, 200);
    CONFERIR(c.estado == ESTADO_PATRULHA || c.estado == ESTADO_PREPARACAO);

    printf("6) Saude e removido da lista ao chegar a zero\n");
    ListaInimigos lista;
    InicializarLista(&lista);
    NoInimigo *no = InserirInimigo(&lista, CriarCaranguejo(3000.0f, 620.0f, 2970.0f, 3270.0f));
    CONFERIR(no->dados.saude == CARANGUEJO_SAUDE);
    no->dados.saude = 0;
    AtualizarInimigos(&lista, &longe, DT);
    CONFERIR(lista.quantidade == 0 && lista.inicio == NULL);

    if (falhas == 0) printf("\nOK: todos os testes do caranguejo passaram.\n");
    else printf("\n%d verificacao(oes) falharam.\n", falhas);
    return falhas;
}
