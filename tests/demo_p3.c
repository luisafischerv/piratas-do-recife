#include "raylib.h"
#include "jogador.h"
#include "inimigos.h"
#include "inimigos_ia.h"
#include "marujo.h"

#define SAUDE_MAX 3
#define PROTECAO_APOS_DANO 1.0f

static int saude = SAUDE_MAX;
static float protecao = 0.0f;

static void DanoProvisorio(int dano)
{
    if (dano <= 0 || protecao > 0.0f) return;
    saude -= 1;
    protecao = PROTECAO_APOS_DANO;
}

static void CriarMarujosDemo(ListaInimigos *lista, float chaoY)
{
    InserirInimigo(lista, CriarMarujo(500.0f, chaoY, 350.0f, 650.0f));
    InserirInimigo(lista, CriarMarujo(850.0f, chaoY, 750.0f, 1000.0f));
}

static void Reiniciar(Jogador *nico, ListaInimigos *lista, float chaoY)
{
    LiberarInimigos(lista);
    CriarMarujosDemo(lista, chaoY);
    nico->corpo.x = 100.0f;
    nico->corpo.y = 400.0f;
    nico->velocidadeY = 0.0f;
    saude = SAUDE_MAX;
    protecao = 0.0f;
}

int main(void)
{
    InitWindow(1280, 720, "Demo Pessoa 3 - marujos");
    SetTargetFPS(60);

    const float chaoY = (float)GetScreenHeight() - 100.0f;

    Jogador nico;
    InicializarJogador(&nico);

    ListaInimigos lista;
    InicializarLista(&lista);
    CriarMarujosDemo(&lista, chaoY);
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        AtualizarJogador(&nico, dt);
        DanoProvisorio(AtualizarInimigos(&lista, &nico, dt));
        if (protecao > 0.0f) protecao -= dt;

        if (saude <= 0 || IsKeyPressed(KEY_R)) Reiniciar(&nico, &lista, chaoY);
        BeginDrawing();
        ClearBackground((Color){ 16, 43, 58, 255 });
        DrawRectangle(0, (int)chaoY, GetScreenWidth(), 100, DARKGRAY);

        DesenharInimigos(&lista);
        if (protecao <= 0.0f || ((int)(protecao * 10) % 2) == 0)
            DesenharJogador(&nico);
        DrawText(TextFormat("Saude: %d   Inimigos na lista: %d", saude, lista.quantidade),
                 24, 24, 22, RAYWHITE);
        DrawText("A/D: mover   ESPACO: pular   R: reiniciar", 24, 54, 20, LIGHTGRAY);
        EndDrawing();
    }
    LiberarInimigos(&lista);
    DescarregarJogador(&nico);
    CloseWindow();
    return 0;
}