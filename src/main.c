#include "jogador.h"
#include "fase.h"
#include "colisao.h"
#include "combate.h"
#include "inimigos_ia.h"
#include "marujo.h"

int main(void)
{
    InitWindow(1280, 720, "Piratas do Recife: A Maldicao da Mare Negra");
    SetTargetFPS(60);

    Jogador nico;
    InicializarJogador(&nico);
    Fase fase;
    CarregarFase(&fase, 1);
    ListaInimigos inimigos;
    InicializarLista(&inimigos);
    InserirInimigo(&inimigos, CriarMarujo(650.0f, 620.0f, 380.0f, 860.0f));
    InserirInimigo(&inimigos, CriarMarujo(1050.0f, 620.0f, 920.0f, 1280.0f));
    Espada espada;
    InicializarEspada(&espada);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (dt > 1.0f / 30.0f) dt = 1.0f / 30.0f;

        Rectangle anterior = nico.corpo;
        AtualizarJogador(&nico, dt);
        ResolverColisoesCenario(&nico, &fase, anterior);
        if (AtualizarAtaqueEspada(&espada, IsMouseButtonPressed(MOUSE_BUTTON_LEFT), dt))
            IniciarAnimacaoAtaque(&nico);
        VerificarAcertosEspada(&espada, &nico, &inimigos);
        AtualizarInimigos(&inimigos, &nico, dt);

        BeginDrawing();
        ClearBackground((Color){ 16, 43, 58, 255 });

        DesenharFase(&fase);
        DrawText("A / D: mover     ESPACO: pular     MOUSE ESQ: espada     ESC: sair", 24, 24, 22, RAYWHITE);

        DesenharInimigos(&inimigos);
        DesenharJogador(&nico);

        EndDrawing();
    }

    LiberarInimigos(&inimigos);
    DescarregarJogador(&nico);
    CloseWindow();
    return 0;
}
