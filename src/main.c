#include "jogador.h"
#include "fase.h"

int main(void)
{
    InitWindow(1280, 720, "Piratas do Recife: A Maldicao da Mare Negra");
    SetTargetFPS(60);

    Jogador nico;
    InicializarJogador(&nico);
    Fase fase;
    CarregarFase(&fase, 1);

    while (!WindowShouldClose()) {
        const float dt = GetFrameTime();

        AtualizarJogador(&nico, dt);

        BeginDrawing();
        ClearBackground((Color){ 16, 43, 58, 255 });

        DesenharFase(&fase);
        DrawText("A / D: mover     ESPACO: pular     ESC: sair", 24, 24, 22, RAYWHITE);

        DesenharJogador(&nico);

        EndDrawing();
    }

    DescarregarJogador(&nico);
    CloseWindow();
    return 0;
}
