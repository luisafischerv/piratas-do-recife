#include "jogador.h"
#include "fase.h"
#include "colisao.h"
#include "combate.h"
#include "inimigos_ia.h"
#include "marujo.h"
#include "interface.h"
#include "tiros.h"
#include "eventos.h"
#include "cipo.h"
#include "cordas.h"
#include "canhoes_escalada.h"
#include "provisorio/troca_fase.h"
#include <math.h>

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
    Inimigo marujo = CriarMarujo(700.0f, 620.0f, 400.0f, 1420.0f);
    marujo.aguardaProximidade = true;
    InserirInimigo(inimigos, marujo);
    Inimigo atirador = CriarAtirador(1080.0f, 620.0f);
    atirador.limiteEsquerdo = 900.0f;
    atirador.limiteDireito = 1260.0f;
    atirador.aguardaProximidade = true;
    InserirInimigo(inimigos, atirador);
    marujo = CriarMarujo(1320.0f, 620.0f, 400.0f, 1420.0f);
    marujo.aguardaProximidade = true;
    InserirInimigo(inimigos, marujo);
    // Os limites mantem o atirador na passarela de 1600 a 1860.
    atirador = CriarAtirador(1730.0f, 480.0f);
    atirador.limiteEsquerdo = 1600.0f;
    atirador.limiteDireito = 1860.0f;
    atirador.aguardaProximidade = true;
    InserirInimigo(inimigos, atirador);
    // Marujo patrulha o espaco aberto sob as passarelas da segunda tela.
    marujo = CriarMarujo(1870.0f, 620.0f, 1570.0f, 2050.0f);
    marujo.aguardaProximidade = true;
    InserirInimigo(inimigos, marujo);
    // Segundo atirador elevado fica limitado a passarela mais alta.
    atirador = CriarAtirador(2000.0f, 435.0f);
    atirador.limiteEsquerdo = 1950.0f;
    atirador.limiteDireito = 2120.0f;
    atirador.aguardaProximidade = true;
    InserirInimigo(inimigos, atirador);
    atirador = CriarAtirador(2260.0f, 620.0f);
    atirador.limiteEsquerdo = 2110.0f;
    atirador.limiteDireito = 2520.0f;
    atirador.aguardaProximidade = true;
    InserirInimigo(inimigos, atirador);
}

static void CriarInimigosDaFase(ListaInimigos *inimigos, int faseId)
{
    if (faseId == 1) {
        CriarMarujosDoCais(inimigos);
    } else if (faseId == 2) {
        Inimigo atirador = CriarAtirador(1090.0f, 620.0f);
        atirador.limiteEsquerdo = 1090.0f;
        atirador.limiteDireito = 1134.0f;
        atirador.aguardaProximidade = true;
        InserirInimigo(inimigos, atirador);
        atirador = CriarAtirador(1500.0f, 620.0f);
        atirador.limiteEsquerdo = 1500.0f;
        atirador.limiteDireito = 1544.0f;
        atirador.aguardaProximidade = true;
        InserirInimigo(inimigos, atirador);
        atirador = CriarAtirador(3100.0f, 620.0f);
        atirador.limiteEsquerdo = 2960.0f;
        atirador.limiteDireito = 3280.0f;
        atirador.aguardaProximidade = true;
        InserirInimigo(inimigos, atirador);
    }
}

static void AtivarInimigosProximos(ListaInimigos *inimigos, const Jogador *nico)
{
    for (NoInimigo *no = inimigos->inicio; no; no = no->proximo) {
        Inimigo *inimigo = &no->dados;
        if (inimigo->aguardaProximidade &&
            fabsf(CentroXRetangulo(inimigo->corpo) - CentroXRetangulo(nico->corpo)) <= 700.0f)
            inimigo->aguardaProximidade = false;
    }
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
    CarregarVisualTiros();
    SistemaCipos cipos;
    CarregarCipos(&cipos);
    SistemaCordas cordas;
    CarregarCordas(&cordas);
    CanhoesEscalada canhoesEscalada;
    ReiniciarCanhoesEscalada(&canhoesEscalada);

    Jogador nico;
    InicializarJogador(&nico);
    const Jogador nicoInicial = nico;
    Fase fase;
    CarregarFase(&fase, 1);
    Camera2D camera = {0};
    camera.zoom = 1.0f;
    ListaInimigos inimigos;
    InicializarLista(&inimigos);
    CriarInimigosDaFase(&inimigos, fase.id);
    Espada espada;
    InicializarEspada(&espada);
    SistemaTiros tiros;
    InicializarTiros(&tiros);
    SequenciaEventos eventos;   // eventos da fase, ordenados por Insertion Sort
    MontarEventosDaFase(&eventos, fase.id);

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
            int destinoTeste = DestinoTesteFase(fase.id);
            tempoTotal += tempoQuadro;

            float dt = tempoQuadro;
            if (dt > 1.0f / 30.0f) dt = 1.0f / 30.0f;

            Rectangle anterior = nico.corpo;
            if ((fase.id != 2 || cipos.agarrado < 0) && (fase.id != 3 || cordas.agarrada < 0)) AtualizarJogador(&nico, dt);
            if (fase.id == 2) {
                float direcaoCipo = (float)IsKeyDown(KEY_D) - (float)IsKeyDown(KEY_A);
                AtualizarCipos(&cipos, &nico, dt, direcaoCipo,
                               IsKeyDown(KEY_E), IsKeyPressed(KEY_SPACE));
            }
            if (fase.id == 3) {
                int vertical = (int)IsKeyDown(KEY_S) - (int)IsKeyDown(KEY_W);
                int horizontal = (int)IsKeyDown(KEY_D) - (int)IsKeyDown(KEY_A);
                AtualizarCordas(&cordas, &nico, dt, vertical, horizontal,
                                IsKeyDown(KEY_E), IsKeyPressed(KEY_SPACE));
            }
            if (nico.corpo.x < 0.0f) nico.corpo.x = 0.0f;
            if (nico.corpo.x + nico.corpo.width > fase.largura)
                nico.corpo.x = fase.largura - nico.corpo.width;
            if (cipos.agarrado < 0 && (fase.id != 3 || cordas.agarrada < 0)) ResolverColisoesCenario(&nico, &fase, anterior);
            else {
                // Um cipo nao permite atravessar obstaculos solidos.
                for (int p = 0; p < fase.qtdPlataformas; p++) {
                    if (fase.plataformas[p].tipo == PLAT_RAIZ &&
                        CheckCollisionRecs(nico.corpo, fase.plataformas[p].area)) {
                        nico.corpo = anterior;
                        nico.velocidadeY = 0;
                        cipos.agarrado = -1;
                        cipos.impulsoX = 0;
                        cipos.bloqueio = 0.35f;
                        break;
                    }
                }
            }
            AtualizarPlataformasFrageis(&fase, &nico, dt);
            if (AtualizarAtaqueEspada(&espada, cipos.agarrado < 0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT), dt))
                IniciarAnimacaoAtaque(&nico);
            VerificarAcertosEspada(&espada, &nico, &inimigos);
            ExecutarEventos(&eventos, dt, &inimigos, NULL, 0);
            AtivarInimigosProximos(&inimigos, &nico);
            if (tempoProtecao > 0.0f) {
                tempoProtecao -= dt;
            }

            if (AtualizarPistola(&tiros, &nico,
                                 fase.id >= 2 && cipos.agarrado < 0 && IsMouseButtonDown(MOUSE_BUTTON_RIGHT), dt))
                IniciarAnimacaoTiro(&nico);
            AtualizarAtiradores(&tiros, &inimigos, &nico, dt);
            int danoRecebido = 0;
            if (fase.id == 3)
                danoRecebido += AtualizarCanhoesEscalada(&canhoesEscalada, &tiros, &inimigos, &nico, dt);
            danoRecebido += AtualizarTiros(&tiros, &fase, &inimigos, &nico, dt);
            // So um disparo que realmente causa dano derruba Nico da corda.
            if (fase.id == 3 && danoRecebido > 0 && tempoProtecao <= 0.0f && saudeNico > 0)
                SoltarCordaPorDano(&cordas, &nico);
            // AtualizarInimigos remove cada inimigo derrotado uma unica vez.
            int quantidadeAntes = inimigos.quantidade;
            danoRecebido += AtualizarInimigos(&inimigos, &nico, dt);
            int derrotados = quantidadeAntes - inimigos.quantidade;
            AplicarDanoJogador(&saudeNico, &tempoProtecao, danoRecebido);
            if (saudeNico > 0 && nico.corpo.y <= fase.limiteQueda) {
                saudeNico += derrotados;
                if (saudeNico > SAUDE_MAXIMA) saudeNico = SAUDE_MAXIMA;
            }

            if (saudeNico <= 0 || nico.corpo.y > fase.limiteQueda) {
                RestaurarPlataformas(&fase);
                nico = nicoInicial;
                nico.corpo.x = fase.pontoInicial.x;
                nico.corpo.y = fase.pontoInicial.y;
                saudeNico = SAUDE_MAXIMA;
                tempoProtecao = 0.0f;
                InicializarEspada(&espada);
                InicializarTiros(&tiros);
                ReiniciarCipos(&cipos);
                ReiniciarCordas(&cordas);
                ReiniciarCanhoesEscalada(&canhoesEscalada);

                LiberarInimigos(&inimigos);
                InicializarLista(&inimigos);
                CriarInimigosDaFase(&inimigos, fase.id);
                ReiniciarEventos(&eventos);   // relogio local volta a 0, nada duplica
            } else if (destinoTeste != 0 ||
                       ((fase.id == 1 || fase.id == 2) && CheckCollisionRecs(nico.corpo, fase.areaSaida))) {
                Fase proxima = {0};
                int destino = destinoTeste ? destinoTeste : fase.id + 1;
                if (CarregarFase(&proxima, destino)) {
                    DescarregarFase(&fase);
                    fase = proxima;
                    nico = nicoInicial;
                    nico.corpo.x = fase.pontoInicial.x;
                    nico.corpo.y = fase.pontoInicial.y;
                    saudeNico = SAUDE_MAXIMA;
                    tempoProtecao = 0.0f;
                    InicializarEspada(&espada);
                    InicializarTiros(&tiros);
                    ReiniciarCipos(&cipos);
                    ReiniciarCordas(&cordas);
                    ReiniciarCanhoesEscalada(&canhoesEscalada);
                    LiberarInimigos(&inimigos);
                    InicializarLista(&inimigos);
                    CriarInimigosDaFase(&inimigos, fase.id);
                    MontarEventosDaFase(&eventos, fase.id);
                }
            }
        }

        BeginDrawing();
        ClearBackground((Color){ 16, 43, 58, 255 });

        float cameraX = nico.corpo.x + nico.corpo.width / 2.0f - GetScreenWidth() / 2.0f;
        float limiteCamera = fase.largura - GetScreenWidth();
        if (limiteCamera < 0.0f) limiteCamera = 0.0f;
        if (cameraX < 0.0f) cameraX = 0.0f;
        if (cameraX > limiteCamera) cameraX = limiteCamera;
        float cameraY = 0;
        if (fase.id == 3) {
            cameraY = nico.corpo.y + nico.corpo.height / 2 - GetScreenHeight() / 2.0f;
            if (cameraY < 0) cameraY = 0;
            if (cameraY > 1920 - GetScreenHeight()) cameraY = 1920 - GetScreenHeight();
        }
        camera.target = (Vector2){cameraX, cameraY};
        BeginMode2D(camera);
        DesenharFase(&fase);
        if (fase.id == 2) DesenharCipos(&cipos);
        if (fase.id == 3) DesenharCordas(&cordas);
        if (fase.id == 3) DesenharCanhoesEscalada(&canhoesEscalada);
        if (fase.id == 1)
            DrawText("MANGUE >", 2400, 500, 20, GOLD);
        DesenharInimigos(&inimigos);
        DesenharTiros(&tiros);
        bool nicoVisivel = tempoProtecao <= 0.0f || (int)(tempoProtecao * 10.0f) % 2 == 0;
        if (nicoVisivel) {
            if (fase.id == 2 && cipos.agarrado >= 0) DesenharNicoNoCipo(&cipos, &nico);
            else if (fase.id == 3 && cordas.agarrada >= 0) DesenharNicoNaCorda(&cordas, &nico);
            else DesenharJogador(&nico);
        }
        EndMode2D();
        DrawText("TESTE: setas esquerda/direita trocam de fase", 760, 56, 16, GOLD);
        DrawText(fase.id == 1 ?
                 "A/D: mover  ESPACO: pular  MOUSE ESQ: espada  ESC: pausar" :
                 "A/D: mover  ESPACO: pular  MOUSE ESQ: espada  DIR: pistola  ESC: pausar",
                 24, 24, 20, RAYWHITE);
        if (fase.id == 2 && nico.corpo.x < 500.0f)
            DrawText("SUBA: pule segurando E; D da embalo; ESPACO solta o cipo", 24, 130, 20, GOLD);
        if (fase.id == 2 && nico.corpo.x >= 500 && nico.corpo.x < 1010)
            DrawText("SALTE do cipo ate a plataforma abaixo das raizes!", 24, 130, 20, GOLD);
        if (fase.id == 2 && nico.corpo.x >= 1010 && nico.corpo.x <= 1450)
            DrawText("REVIDE: botao direito atira. Use os apoios firmes entre os saltos.", 24, 130, 20, GOLD);
        if (fase.id == 2 && nico.corpo.x > 1450.0f)
            DrawText("CIPOS: segure E para agarrar; A/D dao embalo; ESPACO solta", 24, 130, 20, GOLD);
        if (fase.id == 2 && nico.corpo.x > 1450.0f && nico.corpo.x < 2960.0f)
            DrawText("Solte durante a subida para alcancar o proximo cipo!", 24, 158, 18, RAYWHITE);
        if (fase.id == 3)
            DrawText("E: agarrar  W/S: subir/descer  A/D + ESPACO: saltar", 24, 130, 20, GOLD);
        if (fase.id == 3 && nico.corpo.y <= 200)
            DrawText("CONVES ALCANCADO - proxima etapa: chefe final", 24, 158, 20, GOLD);

        for (int i = 0; i < SAUDE_MAXIMA; i++) {
            DesenharCoracao(24 + i * 40, 56, 4, i < saudeNico);
        }

        int segundos = (int)tempoTotal;
        DrawText(TextFormat("Tempo: %02d:%02d", segundos / 60, segundos % 60),
                 24, 96, 22, RAYWHITE);

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
    DescarregarFase(&fase);
    DescarregarVisualInimigos();
    DescarregarVisualTiros();
    DescarregarCipos(&cipos);
    DescarregarCordas(&cordas);
    DescarregarJogador(&nico);
    if (fundoInicial.id != 0) {
        UnloadTexture(fundoInicial);
    }
    CloseWindow();
    return 0;
}
