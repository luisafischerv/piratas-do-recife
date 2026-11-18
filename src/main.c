#include "jogador.h"
#include "fase.h"
#include "colisao.h"
#include "combate.h"
#include "inimigos_ia.h"
#include "marujo.h"
#include "interface.h"
#include "tiros.h"
#include "cipo.h"
#include "cordas.h"
#include "canhoes_escalada.h"
#include "avelino.h"
#include "bando_caranguejos.h"
#include "provisorio/troca_fase.h"
#include <math.h>
#include "ranking.h"

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
    marujo = CriarMarujo(1870.0f, 620.0f, 1570.0f, 2050.0f);
    marujo.aguardaProximidade = true;
    InserirInimigo(inimigos, marujo);
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
        atirador = CriarAtirador(3200.0f, 620.0f);
        atirador.limiteEsquerdo = 3120.0f;
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

    if (!fundoInicial.id) {
        DrawText(titulo, centroX - MeasureText(titulo, 56) / 2, yTitulo, 56, GOLD);
        DrawText(subtitulo, centroX - MeasureText(subtitulo, 30) / 2, ySubtitulo, 30, RAYWHITE);
    }
    DrawRectangle(centroX - 190, yDica - 10, 380, 45, Fade(BLACK, 0.7f));
    DrawText(dica, centroX - MeasureText(dica, 24) / 2,
             yDica, 24, RAYWHITE);
}

static void DesenharIntroducao(Texture2D imagem) {
    if (imagem.id) {
        Rectangle origem = {0, 0, (float)imagem.width, (float)imagem.height};
        Rectangle destino = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
        DrawTexturePro(imagem, origem, destino, (Vector2){0, 0}, 0, WHITE);
    } else {
        DrawText("A Mare Negra cobriu o Recife.", 80, 260, 30, RAYWHITE);
        DrawText("Nico parte para enfrentar o capitao Avelino.", 80, 310, 30, RAYWHITE);
        DrawText("Pressione ENTER para continuar", 80, 390, 24, RAYWHITE);
    }
}

static bool AvelinoDerrotado(const Avelino *avelino){
    return avelino->saude <= 0;
}

static void DesenharFinalIlustrado(Texture2D imagem){
    int largura = GetScreenWidth();
    int altura = GetScreenHeight();

    if (imagem.id != 0) {
        Rectangle origem = {
            0, 0, (float)imagem.width, (float)imagem.height
        };
        Rectangle destino = { 0, 0, (float)largura, (float)altura };

        DrawTexturePro(imagem, origem, destino, (Vector2){ 0, 0 }, 0.0f, WHITE);
        return;
    }

    DrawRectangle(0, altura - 180, largura, 180, Fade(BLACK, 0.85f));
    DrawText("VITORIA!", 48, altura - 155, 40, GOLD);
    DrawText("Nico derrotou Avelino. A nevoa recuou e a luz voltou ao Recife.",
             48, altura - 99, 24, RAYWHITE);

    DrawText("Pressione Enter para ver o resultado",
             48, altura - 55, 22, GOLD);
}

static void AtualizarApelido(char apelido[TAM_APELIDO], int *tamanho){
    if (IsKeyPressed(KEY_BACKSPACE) && *tamanho > 0) {
        (*tamanho)--;
        apelido[*tamanho] = '\0';
    }

    int caractere;
    while ((caractere = GetCharPressed()) != 0) {
        if (caractere >= 32 && caractere <= 126 && caractere != ';' &&
            *tamanho < TAM_APELIDO - 1) {
            apelido[*tamanho] = (char)caractere;
            (*tamanho)++;
            apelido[*tamanho] = '\0';
        }
    }
}

int main(void)
{
    InitWindow(1280, 720, "Piratas do Recife: A Maldicao da Mare Negra");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    Texture2D fundoInicial = LoadTexture("assets/images/capa_nova_proposta.png");
    Texture2D fundoFinal = LoadTexture("assets/images/final_recife_aprovado.png");
    Texture2D fundoIntroducao = LoadTexture("assets/images/transicoes/historia_leitura_proposta.png");
    Texture2D transicoes[4] = {
        LoadTexture("assets/images/transicoes/cais.png"),
        LoadTexture("assets/images/transicoes/ilha.png"),
        LoadTexture("assets/images/transicoes/escalada.png"),
        LoadTexture("assets/images/transicoes/avelino.png")
    };
    Avelino avelino;
    CarregarAvelino(&avelino);
    BandoCaranguejos bando;
    ReiniciarBando(&bando);
    CarregarVisualInimigos();
    CarregarVisualTiros();
    CarregarVisualCanhoes();
    CarregarVisualEscalada();
    CarregarVisualBando();
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

    int saudeNico = SAUDE_MAXIMA;
    float tempoProtecao = 0.0f;
    bool pausado = false;
    double tempoTotal = 0.0;
    bool campanhaConcluida = false;
    bool naTelaResultado = false;
    bool rankingExibido = false;
    char apelido[TAM_APELIDO] = "";
    int tamanhoApelido = 0;
    Ranking ranking;
    const char *arquivoRanking = ARQUIVO_RANKING;
    CarregarRanking(&ranking, arquivoRanking);

    const char *mensagemRanking = "";
    bool naTelaInicial = true;
    bool naIntroducao = false;
    bool naTransicao = true;
    while (!WindowShouldClose()) {

        if (campanhaConcluida) {
            if (!naTelaResultado) {
                if (IsKeyPressed(KEY_ENTER)) {
                    naTelaResultado = true;
                }

                BeginDrawing();
                ClearBackground((Color){ 16, 43, 58, 255 });
                DesenharFinalIlustrado(fundoFinal);
                EndDrawing();
                continue;
            }

            if (!rankingExibido) {
                AtualizarApelido(apelido, &tamanhoApelido);
                if (IsKeyPressed(KEY_ENTER) && tamanhoApelido > 0) {
                    bool entrou = RegistrarResultado(&ranking, apelido, tempoTotal);
                    if (entrou && !SalvarRanking(&ranking, arquivoRanking)) {
                        CarregarRanking(&ranking, arquivoRanking);
                        mensagemRanking = "Nao foi possivel salvar. Pressione Enter para tentar novamente.";
                    } else {
                        mensagemRanking = entrou ? "Resultado salvo!" : "Seu tempo ficou fora dos 10 melhores.";
                        rankingExibido = true;
                    }
                }
            } else if (IsKeyPressed(KEY_ENTER)) {
                Fase primeira = {0};
                if (!CarregarFase(&primeira, 1)) {
                    mensagemRanking = "Nao foi possivel reiniciar a campanha.";
                } else {
                    DescarregarFase(&fase);
                    fase = primeira;
                    nico = nicoInicial;
                    nico.corpo.x = fase.pontoInicial.x;
                    nico.corpo.y = fase.pontoInicial.y;
                    saudeNico = SAUDE_MAXIMA;
                    tempoProtecao = 0.0f;
                    tempoTotal = 0.0;
                    InicializarEspada(&espada);
                    InicializarTiros(&tiros);
                    ReiniciarCipos(&cipos);
                    ReiniciarCordas(&cordas);
                    ReiniciarCanhoesEscalada(&canhoesEscalada);
                    ReiniciarAvelino(&avelino);
                    ReiniciarBando(&bando);
                    LiberarInimigos(&inimigos);
                    InicializarLista(&inimigos);
                    CriarInimigosDaFase(&inimigos, fase.id);
                    campanhaConcluida = false;
                    naTelaResultado = false;
                    rankingExibido = false;
                    apelido[0] = '\0';
                    tamanhoApelido = 0;
                    mensagemRanking = "";
                    pausado = false;
                    naTelaInicial = true;
                    naIntroducao = false;
                    naTransicao = true;
                    BeginDrawing();
                    ClearBackground((Color){16, 43, 58, 255});
                    DesenharTelaInicial(fundoInicial);
                    EndDrawing();
                    continue;
                }
            }

            BeginDrawing();
            ClearBackground((Color){ 16, 43, 58, 255 });
            DrawText("VITORIA!", 48, 48, 48, GOLD);

            char tempoFormatado[32];
            FormatarTempo(tempoTotal, tempoFormatado, sizeof tempoFormatado);
            DrawText(TextFormat("Tempo final: %s", tempoFormatado),
                     48, 115, 28, RAYWHITE);

            if (rankingExibido) {
                DrawText(mensagemRanking, 48, 155, 22, RAYWHITE);
                DesenharRanking(&ranking, 48, 200);
                DrawText("Pressione Enter para voltar ao inicio", 48, 655, 22, GOLD);
            } else {
                DrawText("Digite seu apelido (ate 12 caracteres)", 48, 175, 24, RAYWHITE);
                DrawRectangleLines(48, 213, 360, 50, GOLD);
                DrawText(apelido, 60, 225, 26, RAYWHITE);
                DrawText("Pressione Enter para confirmar", 48, 280, 22, GOLD);
                if (mensagemRanking[0]) DrawText(mensagemRanking, 48, 320, 20, RAYWHITE);
            }

            EndDrawing();
            continue;
        }

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
            DesenharIntroducao(fundoIntroducao);
            EndDrawing();
            continue;
        }

        float tempoQuadro = GetFrameTime();

        if (naTransicao) {
            Texture2D tela = transicoes[fase.id - 1];
            if (IsKeyPressed(KEY_ENTER)) naTransicao = false;
            BeginDrawing();
            ClearBackground((Color){16, 43, 58, 255});
            if (tela.id) {
                Rectangle origem = {0, 0, (float)tela.width, (float)tela.height};
                Rectangle destino = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
                DrawTexturePro(tela, origem, destino, (Vector2){0, 0}, 0, WHITE);
            } else {
                DrawText("Pressione ENTER para iniciar", 380, 350, 30, RAYWHITE);
            }
            EndDrawing();
            continue;
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            pausado = !pausado;
        }

        if (!pausado) {
            int destinoTeste = DestinoTesteFase(fase.id);
            if (destinoTeste == 5) {
                campanhaConcluida = true;
                continue;
            }
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
            AtivarInimigosProximos(&inimigos, &nico);
            if (tempoProtecao > 0.0f) {
                tempoProtecao -= dt;
            }

            if (AtualizarPistola(&tiros, &nico,
                                 fase.id >= 2 && cipos.agarrado < 0 && IsMouseButtonDown(MOUSE_BUTTON_RIGHT), dt))
                IniciarAnimacaoTiro(&nico);
            AtualizarAtiradores(&tiros, &inimigos, &nico, dt);
            int danoRecebido = 0;
            if (fase.id == 4) {
                int anteriorEstagio = EstagioAvelino(&avelino);
                danoRecebido += AtualizarAvelino(&avelino, &nico, &espada, &tiros, dt);
                if (AvelinoDerrotado(&avelino)) {
                    campanhaConcluida = true;
                    continue;
                }
                if (avelino.saude > 0 && anteriorEstagio < 3 && EstagioAvelino(&avelino) == 3)
                    ConvocarMarujos(&inimigos, 2, 2, 600, 0, fase.largura, nico.corpo.x);
            }
            if (fase.id == 4 && avelino.saude > 0)
                danoRecebido += AtualizarBando(&bando, &nico, EstagioAvelino(&avelino), dt);
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
            if (fase.id != 4 && saudeNico > 0 && nico.corpo.y <= fase.limiteQueda) {
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
                ReiniciarAvelino(&avelino);
                ReiniciarBando(&bando);

                LiberarInimigos(&inimigos);
                InicializarLista(&inimigos);
                CriarInimigosDaFase(&inimigos, fase.id);
            } else if (destinoTeste != 0 ||
                       ((fase.id == 1 || fase.id == 2) && inimigos.quantidade == 0 && CheckCollisionRecs(nico.corpo, fase.areaSaida)) ||
                       (fase.id == 3 && nico.corpo.y <= 200)) {
                Fase proxima = {0};
                int destino = destinoTeste ? destinoTeste : fase.id + 1;
                if (CarregarFase(&proxima, destino)) {
                    DescarregarFase(&fase);
                    fase = proxima;
                    naTransicao = true;
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
                    ReiniciarAvelino(&avelino);
                    ReiniciarBando(&bando);
                    LiberarInimigos(&inimigos);
                    InicializarLista(&inimigos);
                    CriarInimigosDaFase(&inimigos, fase.id);
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
        if (fase.id == 4) DesenharAvelino(&avelino);
        if (fase.id == 4 && avelino.saude > 0) DesenharBando(&bando);
        if (fase.id == 2) DesenharCipos(&cipos);
        if (fase.id == 3) DesenharCordas(&cordas);
        if (fase.id == 3) DesenharCanhoesEscalada(&canhoesEscalada);
        DesenharInimigos(&inimigos);
        DesenharTiros(&tiros);
        bool nicoVisivel = tempoProtecao <= 0.0f || (int)(tempoProtecao * 10.0f) % 2 == 0;
        if (nicoVisivel) {
            if (fase.id == 2 && cipos.agarrado >= 0) DesenharNicoNoCipo(&cipos, &nico);
            else if (fase.id == 3 && cordas.agarrada >= 0) DesenharNicoNaCorda(&cordas, &nico);
            else DesenharJogador(&nico);
        }
        EndMode2D();
        if (fase.id == 1 || fase.id == 2) {
            DrawText(TextFormat("Inimigos restantes: %d", inimigos.quantidade),
                     24, 24, 20, RAYWHITE);
            if (inimigos.quantidade > 0 && CheckCollisionRecs(nico.corpo, fase.areaSaida)) {
                const char *mensagem = "Derrote todos os inimigos para avancar";
                int largura = MeasureText(mensagem, 22);
                DrawRectangle((GetScreenWidth() - largura) / 2 - 16, 125, largura + 32, 38, Fade(BLACK, 0.8f));
                DrawText(mensagem, (GetScreenWidth() - largura) / 2, 133, 22, RAYWHITE);
            }
        }

        for (int i = 0; i < SAUDE_MAXIMA; i++) {
            DesenharCoracao(24 + i * 40, 56, 4, i < saudeNico);
        }

        int segundos = (int)tempoTotal;
        DrawText(TextFormat("Tempo: %02d:%02d", segundos / 60, segundos % 60), 24, 96, 22, RAYWHITE);


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
    DescarregarAvelino(&avelino);
    DescarregarVisualInimigos();
    DescarregarVisualTiros();
    DescarregarVisualCanhoes();
    DescarregarVisualEscalada();
    DescarregarVisualBando();
    DescarregarCipos(&cipos);
    DescarregarCordas(&cordas);
    DescarregarJogador(&nico);
    for (int i = 0; i < 4; i++) if (transicoes[i].id) UnloadTexture(transicoes[i]);
    if (fundoIntroducao.id) UnloadTexture(fundoIntroducao);
    if (fundoInicial.id != 0) {
        UnloadTexture(fundoInicial);
    }
    if (fundoFinal.id != 0) {
        UnloadTexture(fundoFinal);
    }
    CloseWindow();
    return 0;
}
