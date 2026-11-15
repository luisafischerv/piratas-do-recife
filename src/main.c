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

static void DesenharTelaInicial(Texture2D fundoInicial) {

    const char *titulo = "Piratas do Recife";
    const char *subtitulo = "A Maldição da Maré Negra";
    const char *dica = "Pressione Enter para jogar";
    int centroX = GetScreenWidth() / 2;
    int centroY = GetScreenHeight() / 2;
    int yTitulo = centroY - 100;
    int ySubtitulo = centroY - 32;
    int yDica = centroY + 55;

    if (fundoInicial.id != 0) {
        Rectangle origem = {
            0.0f, 0.0f, (float)fundoInicial.width, (float)fundoInicial.height
        };
        Rectangle destino = {
            0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()
        };

        DrawTexturePro(fundoInicial, origem, destino,
                       (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);

        yTitulo = 90;
        ySubtitulo = 158;
        yDica = GetScreenHeight() - 65;
    }

    DrawText(titulo, centroX - MeasureText(titulo, 56) / 2,
             yTitulo, 56, GOLD);
    DrawText(subtitulo, centroX - MeasureText(subtitulo, 30) / 2,
             ySubtitulo, 30, RAYWHITE);
    DrawText(dica, centroX - MeasureText(dica, 24) / 2,
             yDica, 24, RAYWHITE);
}

static void DesenharIntroducao(Texture2D fundoInicial) {
    int largura = GetScreenWidth();
    int altura = GetScreenHeight();

    if (fundoInicial.id != 0) {
        Rectangle origem = {
            0, 0, (float)fundoInicial.width, (float)fundoInicial.height
        };
        Rectangle destino = { 0, 0, (float)largura, (float)altura };

        DrawTexturePro(fundoInicial, origem, destino,
                       (Vector2){ 0, 0 }, 0.0f, WHITE);
    }

    DrawRectangle(0, altura - 210, largura, 210, Fade(BLACK, 0.82f));

    DrawText("O Capitão Avelino retirou um sino das ruínas do mangue e despertou a Maré Negra.",
             48, altura - 180, 24, RAYWHITE);
    DrawText("Uma névoa avançou sobre o Recife com a tripulação amaldiçoada.",
             48, altura - 145, 24, RAYWHITE);
    DrawText("Nico Maré precisa chegar ao navio e enfrentar Avelino para salvar a cidade.",
             48, altura - 110, 24, RAYWHITE);
    DrawText("Pressione Enter para continuar", 48, altura - 55, 22, GOLD);
}

int main(void)
{
    InitWindow(1280, 720, "Piratas do Recife: A Maldicao da Mare Negra");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    Texture2D fundoInicial = LoadTexture("assets/images/tela_inicial.png");
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
    bool naIntroducao = false;

    while (!WindowShouldClose()) {
        if (naTelaInicial) {
            if (IsKeyPressed(KEY_ENTER)) {
                naTelaInicial = false;
                naIntroducao = true;
            }

            BeginDrawing();
            ClearBackground((Color){ 16, 43, 58, 255 });
            DesenharTelaInicial(fundoInicial);
            EndDrawing();
            continue;
        }

        if (naIntroducao) {
            if (IsKeyPressed(KEY_ENTER)) {
                naIntroducao = false;
            }

            BeginDrawing();
            ClearBackground((Color){ 16, 43, 58, 255 });
            DesenharIntroducao(fundoInicial);
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
    if (fundoInicial.id != 0) {
        UnloadTexture(fundoInicial);
    }
    CloseWindow();
    return 0;
}