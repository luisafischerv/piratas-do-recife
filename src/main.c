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

static void DesenharTelaInicial(void) {

    const char *titulo = "Piratas do Recife";
    const char *subtitulo = "A Maldição da Maré Negra";
    const char *dica = "Pressione Enter para jogar";
    int centroX = GetScreenWidth() / 2;
    int centroY = GetScreenHeight() / 2;

    DrawText(titulo, centroX - MeasureText(titulo, 56) / 2,
             centroY - 100, 56, GOLD);
    DrawText(subtitulo, centroX - MeasureText(subtitulo, 30) / 2,
             centroY - 32, 30, RAYWHITE);
    DrawText(dica, centroX - MeasureText(dica, 24) / 2,
             centroY + 55, 24, RAYWHITE);
}

int main(void)
{
    InitWindow(1280, 720, "Piratas do Recife: A Maldicao da Mare Negra");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
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
    bool pausado = false;
    double tempoTotal = 0.0;
    bool naTelaInicial = true;

    while (!WindowShouldClose()) {

        if (naTelaInicial) {
            if (IsKeyPressed(KEY_ENTER)) {
                naTelaInicial = false;
            }

            BeginDrawing();
            ClearBackground((Color){ 16, 43, 58, 255 });
            DesenharTelaInicial();
            EndDrawing();
            continue;
        }

        float tempoQuadro = GetFrameTime();

        if (IsKeyPressed(KEY_ESCAPE)) {
            pausado = !pausado;
        }

        if (!pausado) {
            tempoTotal += tempoQuadro;

            float dt = tempoQuadro;
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

            if (saudeNico <= 0 || nico.corpo.y > fase.limiteQueda) {
                nico = nicoInicial;
                saudeNico = SAUDE_MAXIMA;
                tempoProtecao = 0.0f;
                InicializarEspada(&espada);

                LiberarInimigos(&inimigos);
                InicializarLista(&inimigos);
                CriarMarujosDoCais(&inimigos);
            }
        }

        BeginDrawing();
        ClearBackground((Color){ 16, 43, 58, 255 });

        DesenharFase(&fase);
        DrawText("A / D: mover     ESPACO: pular     MOUSE ESQ: espada     ESC: pausar", 24, 24, 22, RAYWHITE);

        for (int i = 0; i < SAUDE_MAXIMA; i++) {
            DesenharCoracao(24 + i * 40, 56, 4, i < saudeNico);
        }

        int segundos = (int)tempoTotal;
        DrawText(TextFormat("Tempo: %02d:%02d", segundos / 60, segundos % 60),
                 24, 96, 22, RAYWHITE);

        DesenharInimigos(&inimigos);
        bool nicoVisivel = tempoProtecao <= 0.0f || (int)(tempoProtecao * 10.0f) % 2 == 0;

        if (nicoVisivel) {
            DesenharJogador(&nico);
        }

        if (pausado) {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));

            const char *titulo = "PAUSADO";
            const char *dica = "Pressione Esc para continuar";
            int centroX = GetScreenWidth() / 2;
            int centroY = GetScreenHeight() / 2;

            DrawText(titulo, centroX - MeasureText(titulo, 40) / 2,
                     centroY - 35, 40, RAYWHITE);
            DrawText(dica, centroX - MeasureText(dica, 22) / 2,
                     centroY + 20, 22, RAYWHITE);
        }

        EndDrawing();
    }

    LiberarInimigos(&inimigos);
    DescarregarVisualInimigos();
    DescarregarJogador(&nico);
    CloseWindow();
    return 0;
}