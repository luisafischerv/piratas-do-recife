#include "jogador.h"
#include "fase.h"
#include "colisao.h"
#include "combate.h"
#include "inimigos_ia.h"
#include "marujo.h"
#include "tiros.h"

static void CriarInimigosTeste(ListaInimigos *inimigos)
{
    InicializarLista(inimigos);
    InserirInimigo(inimigos, CriarMarujo(650.0f, 620.0f, 380.0f, 860.0f));
    InserirInimigo(inimigos, CriarMarujo(1050.0f, 620.0f, 920.0f, 1280.0f));
    // Posicionado provisoriamente no chao para testar espada e pistola.
    InserirInimigo(inimigos, CriarAtirador(780.0f, 620.0f));
}

int main(void)
{
    InitWindow(1280, 720, "Piratas do Recife: A Maldicao da Mare Negra");
    SetTargetFPS(60);
    CarregarVisualInimigos();

    Jogador nico;
    InicializarJogador(&nico);
    Fase fase;
    CarregarFase(&fase, 1);
    ListaInimigos inimigos;
    CriarInimigosTeste(&inimigos);
    Espada espada;
    InicializarEspada(&espada);
    SistemaTiros tiros;
    InicializarTiros(&tiros);
    int saude = 3;
    float invulneravel = 0.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (dt > 1.0f / 30.0f) dt = 1.0f / 30.0f;

        Rectangle anterior = nico.corpo;
        AtualizarJogador(&nico, dt);
        ResolverColisoesCenario(&nico, &fase, anterior);
        if (AtualizarAtaqueEspada(&espada, IsMouseButtonPressed(MOUSE_BUTTON_LEFT), dt))
            IniciarAnimacaoAtaque(&nico);
        VerificarAcertosEspada(&espada, &nico, &inimigos);
        if (invulneravel > 0.0f) invulneravel -= dt;
        AtualizarPistola(&tiros, &nico, IsMouseButtonDown(MOUSE_BUTTON_RIGHT), dt);
        AtualizarAtiradores(&tiros, &inimigos, &nico, dt);
        int dano = AtualizarTiros(&tiros, &fase, &inimigos, &nico, dt);
        dano += AtualizarInimigos(&inimigos, &nico, dt);
        if (dano > 0 && invulneravel <= 0.0f) {
            saude--;
            invulneravel = 1.0f;
        }
        if (saude <= 0) {
            saude = 3;
            invulneravel = 0.0f;
            nico.corpo.x = 100.0f;
            nico.corpo.y = 400.0f;
            nico.velocidadeY = 0.0f;
            nico.estaNoChao = false;
            nico.preparoSalto = nico.tempoNoAr = nico.tempoPouso = nico.tempoAtaqueVisual = 0.0f;
            InicializarEspada(&espada);
            InicializarTiros(&tiros);
            LiberarInimigos(&inimigos);
            CriarInimigosTeste(&inimigos);
        }

        BeginDrawing();
        ClearBackground((Color){ 16, 43, 58, 255 });

        DesenharFase(&fase);
        DrawText("A/D: mover  ESPACO: pular  MOUSE ESQ: espada  DIR: pistola  ESC: sair", 24, 24, 20, RAYWHITE);
        DrawText(TextFormat("Saude: %d", saude), 24, 55, 22, RAYWHITE);

        DesenharInimigos(&inimigos);
        DesenharTiros(&tiros);
        DesenharJogador(&nico);

        EndDrawing();
    }

    LiberarInimigos(&inimigos);
    DescarregarVisualInimigos();
    DescarregarJogador(&nico);
    CloseWindow();
    return 0;
}
