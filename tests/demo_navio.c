// Demo da Pessoa 3: canhoes anunciados + eventos ordenados por Insertion Sort.
// Convés provisório; o navio de verdade (fase 3) ainda nao existe.
#include "raylib.h"
#include "jogador.h"
#include "fase.h"
#include "colisao.h"
#include "inimigos_ia.h"
#include "marujo.h"
#include "combate.h"
#include "tiros.h"
#include "eventos.h"
#include "canhao.h"

#define CHAO_Y 620.0f
#define NUM_CANHOES 2
#define SAUDE_MAX 3
#define PROTECAO_APOS_DANO 1.0f

static int saude = SAUDE_MAX;
static float protecao = 0.0f;

// Imita a AplicarDanoJogador do main.c.
static void DanoProvisorio(int dano)
{
    if (dano <= 0 || protecao > 0.0f) return;
    saude -= 1;
    protecao = PROTECAO_APOS_DANO;
}

static void MontarConves(Fase *f)
{
    *f = (Fase){0};
    f->id = 3;
    f->largura = 1280.0f;
    f->limiteQueda = CHAO_Y + 200.0f;
    f->pontoInicial = (Vector2){ 600.0f, 400.0f };
    f->plataformas[0] = (Plataforma){ { 0.0f, CHAO_Y, 1280.0f, 100.0f }, PLAT_CHAO, PLAT_NORMAL, 0.0f, true };
    f->qtdPlataformas = 1;
}

// Cadastrados FORA de ordem de proposito: o Insertion Sort organiza.
static void MontarEventosDemo(SequenciaEventos *seq)
{
    InicializarSequencia(seq);
    CadastrarDisparoCanhao(seq, 6.0f, 0);
    CadastrarDisparoCanhao(seq, 2.0f, 0);
    CadastrarDisparoCanhao(seq, 2.0f, 1);   // empate com o anterior: os dois juntos
    CadastrarSurgimentoMarujo(seq, 3.0f, 900.0f, CHAO_Y, 450.0f, 1100.0f);
    CadastrarDisparoCanhao(seq, 5.0f, 1);
    CadastrarDisparoCanhao(seq, 9.0f, 1);
    CadastrarDisparoCanhao(seq, 9.0f, 0);
    CadastrarSurgimentoMarujo(seq, 10.0f, 300.0f, CHAO_Y, 150.0f, 700.0f);
    CadastrarDisparoCanhao(seq, 13.0f, 1);
    CadastrarDisparoCanhao(seq, 12.0f, 0);
    OrdenarEventos(seq->eventos, seq->quantidade);
    ReiniciarEventos(seq);
}

static void DesenharListaEventos(const SequenciaEventos *seq, int x, int y)
{
    DrawText(TextFormat("Relogio da etapa: %.1f s", seq->relogio), x, y, 20, RAYWHITE);
    y += 28;
    DrawText("Eventos (ordenados por Insertion Sort):", x, y, 18, GOLD);
    y += 24;
    for (int i = 0; i < seq->quantidade; i++) {
        const Evento *e = &seq->eventos[i];
        Color cor = e->executado ? GREEN : LIGHTGRAY;
        const char *texto = (e->tipo == EVENTO_DISPARO_CANHAO)
            ? TextFormat("t=%4.1f  canhao %d   (cadastro %d)", e->instante, e->indiceCanhao, e->ordemCadastro)
            : TextFormat("t=%4.1f  marujo surge (cadastro %d)", e->instante, e->ordemCadastro);
        DrawText(texto, x, y, 18, cor);
        if (i == seq->proximo) DrawText(">", x - 16, y, 18, GOLD);
        y += 22;
    }
}

int main(void)
{
    InitWindow(1280, 720, "Demo Pessoa 3 - canhoes e eventos");
    SetTargetFPS(60);
    CarregarVisualInimigos();
    CarregarVisualTiros();

    Jogador nico;
    InicializarJogador(&nico);
    const Jogador nicoInicial = nico;
    Fase fase;
    MontarConves(&fase);
    nico.corpo.x = fase.pontoInicial.x;
    nico.corpo.y = fase.pontoInicial.y;

    ListaInimigos lista;
    InicializarLista(&lista);
    Espada espada;
    InicializarEspada(&espada);
    SistemaTiros tiros;
    InicializarTiros(&tiros);

    Canhao canhoes[NUM_CANHOES] = {
        CriarCanhao(1190.0f, CHAO_Y, -1),   // direita, atira para a esquerda
        CriarCanhao(30.0f, CHAO_Y, 1)       // esquerda, atira para a direita
    };
    bool disparoCanhao[NUM_CANHOES] = { false, false };

    static SequenciaEventos eventos;
    MontarEventosDemo(&eventos);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (dt > 1.0f / 30.0f) dt = 1.0f / 30.0f;

        Rectangle anterior = nico.corpo;
        AtualizarJogador(&nico, dt);
        if (nico.corpo.x < 0.0f) nico.corpo.x = 0.0f;
        if (nico.corpo.x + nico.corpo.width > fase.largura)
            nico.corpo.x = fase.largura - nico.corpo.width;
        ResolverColisoesCenario(&nico, &fase, anterior);

        if (AtualizarAtaqueEspada(&espada, IsMouseButtonPressed(MOUSE_BUTTON_LEFT), dt))
            IniciarAnimacaoAtaque(&nico);
        VerificarAcertosEspada(&espada, &nico, &lista);

        // Eventos marcam pedidos; os canhoes leem os pedidos e fazem o aviso.
        ExecutarEventos(&eventos, dt, &lista, disparoCanhao, NUM_CANHOES);
        AtualizarCanhoes(canhoes, NUM_CANHOES, disparoCanhao, &tiros, dt);

        if (AtualizarPistola(&tiros, &nico, IsMouseButtonDown(MOUSE_BUTTON_RIGHT), dt))
            IniciarAnimacaoTiro(&nico);
        int dano = AtualizarTiros(&tiros, &fase, &lista, &nico, dt);
        dano += AtualizarInimigos(&lista, &nico, dt);
        DanoProvisorio(dano);
        if (protecao > 0.0f) protecao -= dt;

        // Reinicio: limpa tudo e repete os MESMOS eventos, sem duplicar nada.
        if (saude <= 0 || IsKeyPressed(KEY_R)) {
            LiberarInimigos(&lista);
            ReiniciarEventos(&eventos);
            ReiniciarCanhoes(canhoes, NUM_CANHOES, disparoCanhao);
            InicializarTiros(&tiros);
            InicializarEspada(&espada);
            nico = nicoInicial;
            nico.corpo.x = fase.pontoInicial.x;
            nico.corpo.y = fase.pontoInicial.y;
            saude = SAUDE_MAX;
            protecao = 0.0f;
        }

        BeginDrawing();
        ClearBackground((Color){ 24, 30, 48, 255 });
        DesenharFase(&fase);
        DesenharCanhoes(canhoes, NUM_CANHOES);
        DesenharInimigos(&lista);
        DesenharTiros(&tiros);
        if (protecao <= 0.0f || ((int)(protecao * 10) % 2) == 0)
            DesenharJogador(&nico);

        DrawText(TextFormat("Saude: %d   Inimigos na lista: %d", saude, lista.quantidade),
                 24, 24, 22, RAYWHITE);
        DrawText("A/D: mover  ESPACO: pular (desvia do tiro)  MOUSE ESQ: espada  DIR: pistola  R: reiniciar",
                 24, 54, 18, LIGHTGRAY);
        DesenharListaEventos(&eventos, 760, 90);
        EndDrawing();
    }

    LiberarInimigos(&lista);
    DescarregarVisualInimigos();
    DescarregarVisualTiros();
    DescarregarJogador(&nico);
    CloseWindow();
    return 0;
}
