#include "raylib.h"
#include "jogador.h"
#include "inimigos.h"
#include "inimigos_ia.h"
#include "marujo.h"
#include "combate.h"
#include "fase.h"
#include "colisao.h"

#define SAUDE_MAX 3
#define PROTECAO_APOS_DANO 1.0f
#define CHAO_Y 620.0f   // mesmo valor do CHAO_Y da fase.c (Pessoa 2)

static int saude = SAUDE_MAX;
static float protecao = 0.0f;

// Imita a AplicarDanoJogador (Pessoa 4) ate ela existir.
static void DanoProvisorio(int dano)
{
    if (dano <= 0 || protecao > 0.0f) return;
    saude -= 1;
    protecao = PROTECAO_APOS_DANO;
}

// Patrulhas fora das caixas da fase 1 (x 300-360 e 860-920),
// porque o marujo ainda nao colide com o cenario.
static void CriarMarujosDemo(ListaInimigos *lista)
{
    InserirInimigo(lista, CriarMarujo(500.0f, CHAO_Y, 370.0f, 850.0f));
    InserirInimigo(lista, CriarMarujo(1050.0f, CHAO_Y, 930.0f, 1270.0f));
}

static void Reiniciar(Jogador *nico, ListaInimigos *lista, Espada *espada)
{
    LiberarInimigos(lista);
    CriarMarujosDemo(lista);
    InicializarEspada(espada);
    nico->corpo.x = 100.0f;
    nico->corpo.y = 400.0f;
    nico->velocidadeY = 0.0f;
    saude = SAUDE_MAX;
    protecao = 0.0f;
}

int main(void)
{
    InitWindow(1280, 720, "Demo Pessoa 3 - marujos e espada");
    SetTargetFPS(60);

    Jogador nico;
    InicializarJogador(&nico);

    Fase fase;
    CarregarFase(&fase, 1);

    Espada espada;
    InicializarEspada(&espada);

    ListaInimigos lista;
    InicializarLista(&lista);
    CriarMarujosDemo(&lista);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (dt > 1.0f / 30.0f) dt = 1.0f / 30.0f;

        Rectangle anterior = nico.corpo;
        AtualizarJogador(&nico, dt);
        ResolverColisoesCenario(&nico, &fase, anterior);

        AtualizarAtaqueEspada(&espada, IsMouseButtonPressed(MOUSE_BUTTON_LEFT), dt);
        VerificarAcertosEspada(&espada, &nico, &lista);

        DanoProvisorio(AtualizarInimigos(&lista, &nico, dt));
        if (protecao > 0.0f) protecao -= dt;

        if (saude <= 0 || IsKeyPressed(KEY_R)) Reiniciar(&nico, &lista, &espada);

        BeginDrawing();
        ClearBackground((Color){ 16, 43, 58, 255 });
        DesenharFase(&fase);

        DesenharInimigos(&lista);
        if (protecao <= 0.0f || ((int)(protecao * 10) % 2) == 0)
            DesenharJogador(&nico);
        DesenharEspada(&espada, &nico);

        DrawText(TextFormat("Saude: %d   Inimigos na lista: %d   Golpes: %d",
                            saude, lista.quantidade, espada.idGolpe),
                 24, 24, 22, RAYWHITE);
        DrawText("A/D: mover   ESPACO: pular   CLIQUE ESQ.: espada   R: reiniciar",
                 24, 54, 20, LIGHTGRAY);
        EndDrawing();
    }

    LiberarInimigos(&lista);
    DescarregarJogador(&nico);
    CloseWindow();
    return 0;
}
