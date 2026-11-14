#include "jogador.h"
#include "fase.h"
#include "colisao.h"
#include "combate.h"
#include "inimigos_ia.h"
#include "marujo.h"
#include "interface.h"

#define SAUDE_MAXIMA 3
#define DURACAO_PROTECAO 1.0f

static void AplicarDanoJogador(int *saude, float *tempoProtecao, int dano){
    if (dano <= 0 || *tempoProtecao > 0.0f || *saude <= 0) {
        return;
    }

    (*saude)--;
    *tempoProtecao = DURACAO_PROTECAO;
}

static void CriarMarujosDoCais(ListaInimigos *inimigos){
    InserirInimigo(inimigos, CriarMarujo(650.0f, 620.0f, 380.0f, 860.0f));
    InserirInimigo(inimigos, CriarMarujo(1050.0f, 620.0f, 920.0f, 1280.0f));
}


int main(void)
{
    InitWindow(1280, 720, "Piratas do Recife: A Maldicao da Mare Negra");
    SetTargetFPS(60);
    CarregarVisualInimigos();

    Jogador nico;
    InicializarJogador(&nico);
    const Jogador nicoInicial = nico;
    Fase fase;
    CarregarFase(&fase, 1);
    ListaInimigos inimigos;
    InicializarLista(&inimigos);
    CriarMarujosDoCais(&inimigos);
    Espada espada;
    InicializarEspada(&espada);

    int saudeNico = SAUDE_MAXIMA;
    float tempoProtecao = 0.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (dt > 1.0f / 30.0f) dt = 1.0f / 30.0f;

        Rectangle anterior = nico.corpo;
        AtualizarJogador(&nico, dt);
        ResolverColisoesCenario(&nico, &fase, anterior);
        if (AtualizarAtaqueEspada(&espada, IsMouseButtonPressed(MOUSE_BUTTON_LEFT), dt))
            IniciarAnimacaoAtaque(&nico);
        VerificarAcertosEspada(&espada, &nico, &inimigos);
        if (tempoProtecao > 0.0f) {
            tempoProtecao -= dt;
        }

        int danoRecebido = AtualizarInimigos(&inimigos, &nico, dt);
        AplicarDanoJogador(&saudeNico, &tempoProtecao, danoRecebido);

        if (saudeNico <= 0) {
            nico = nicoInicial;
            saudeNico = SAUDE_MAXIMA;
            tempoProtecao = 0.0f;
            InicializarEspada(&espada);

            LiberarInimigos(&inimigos);
            InicializarLista(&inimigos);
            CriarMarujosDoCais(&inimigos);
        }

        BeginDrawing();
        ClearBackground((Color){ 16, 43, 58, 255 });

        DesenharFase(&fase);
        DrawText("A / D: mover     ESPACO: pular     MOUSE ESQ: espada     ESC: sair", 24, 24, 22, RAYWHITE);

        for (int i = 0; i < SAUDE_MAXIMA; i++) {
            DesenharCoracao(24 + i * 40, 56, 4, i < saudeNico);
        }

        DesenharInimigos(&inimigos);
        bool nicoVisivel = tempoProtecao <= 0.0f || (int)(tempoProtecao * 10.0f) % 2 == 0;

        if (nicoVisivel) {
            DesenharJogador(&nico);
        }

        EndDrawing();
    }

    LiberarInimigos(&inimigos);
    DescarregarVisualInimigos();
    DescarregarJogador(&nico);
    CloseWindow();
    return 0;
}