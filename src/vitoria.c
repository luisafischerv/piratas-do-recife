#include <string.h>
#include "raylib.h"
#include "vitoria.h"

void IniciarVitoria(TelaVitoria *v, double tempoFinal){
    memset(v, 0, sizeof *v);
    v->etapa = VITORIA_FINAL;
    v->tempoFinal = tempoFinal;
    v->posicao = -1;
}

bool ConfirmarApelido(TelaVitoria *v, Ranking *ranking, const char *arquivo){
    if (v->registrado) return false;
    v->registrado = true;

    int ordemNova = ranking->proximaOrdem;
    v->entrouNoRanking = RegistrarResultado(ranking, v->apelido, v->tempoFinal);

    v->posicao = -1;
    for (int i = 0; i < ranking->quantidade; i++) {
        if (ranking->resultados[i].ordemRegistro == ordemNova) {
            v->posicao = i;
            strcpy(v->apelido, ranking->resultados[i].apelido);
            break;
        }
    }
    SalvarRanking(ranking, arquivo);
    return true;
}

static void LerApelido(TelaVitoria *v){
    int c = GetCharPressed();
    while (c > 0) {
        if (c >= 32 && c <= 126 && c != ';' && v->tamanhoApelido < TAM_APELIDO - 1) {
            v->apelido[v->tamanhoApelido++] = (char)c;
            v->apelido[v->tamanhoApelido] = '\0';
        }
        c = GetCharPressed();
    }
    if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) &&
        v->tamanhoApelido > 0) {
        v->apelido[--v->tamanhoApelido] = '\0';
    }
}

bool AtualizarVitoria(TelaVitoria *v, Ranking *ranking){
    switch (v->etapa) {
    case VITORIA_FINAL:
        if (IsKeyPressed(KEY_ENTER)) {
            v->etapa = VITORIA_APELIDO;
            while (GetCharPressed() > 0) { }
        }
        break;
    case VITORIA_APELIDO:
        LerApelido(v);
        if (IsKeyPressed(KEY_ENTER)) {
            ConfirmarApelido(v, ranking, ARQUIVO_RANKING);
            v->etapa = VITORIA_RANKING;
        }
        break;
    case VITORIA_RANKING:
        if (IsKeyPressed(KEY_ENTER)) return true;
        break;
    }
    return false;
}

static void TextoCentralizado(const char *texto, int y, int tamanho, Color cor){
    DrawText(texto, (GetScreenWidth() - MeasureText(texto, tamanho)) / 2, y, tamanho, cor);
}

void DesenharVitoria(const TelaVitoria *v, const Ranking *ranking){
    char tempo[16];
    FormatarTempo(v->tempoFinal, tempo, sizeof tempo);
    int largura = GetScreenWidth(), altura = GetScreenHeight();

    DrawRectangleGradientV(0, 0, largura, altura, (Color){ 16, 43, 58, 255 }, (Color){ 196, 120, 48, 255 });

    if (v->etapa == VITORIA_FINAL){
        TextoCentralizado("VITORIA!", 150, 72, GOLD);
        TextoCentralizado("O Capitao Avelino foi derrotado e a maldicao se rompeu.", 270, 26, RAYWHITE);
        TextoCentralizado("A nevoa recua e a luz volta ao Recife.", 310, 26, RAYWHITE);
        TextoCentralizado(TextFormat("Tempo da campanha: %s", tempo), 400, 32, GOLD);
        TextoCentralizado("Pressione ENTER para continuar", altura - 80, 22, RAYWHITE);
        return;
    }

    if (v->etapa == VITORIA_APELIDO){
        TextoCentralizado("Voce venceu! Registre seu nome no ranking", 170, 32, GOLD);
        TextoCentralizado(TextFormat("Tempo: %s", tempo), 225, 26, RAYWHITE);

        Rectangle caixa = { largura / 2.0f - 220, 300, 440, 64 };
        DrawRectangleRec(caixa, Fade(BLACK, 0.6f));
        DrawRectangleLinesEx(caixa, 3, GOLD);
        bool cursor = ((int)(GetTime() * 2.0) % 2) == 0;
        DrawText(TextFormat("%s%s", v->apelido, cursor ? "_" : ""),
                 (int)caixa.x + 20, (int)caixa.y + 14, 36, RAYWHITE);

        TextoCentralizado(TextFormat("%d/12 caracteres  -  BACKSPACE apaga  -  ENTER confirma", v->tamanhoApelido), 385, 20, RAYWHITE);
        if (v->tamanhoApelido == 0)
            TextoCentralizado("(vazio: sera registrado como \"Pirata\")", 415, 20, LIGHTGRAY);
        return;
    }

    TextoCentralizado(TextFormat("Parabens, %s!", v->apelido), 40, 40, GOLD);
    const char *colocacao;
    if (v->posicao == 0)       colocacao = TextFormat("NOVO RECORDE! 1o lugar com %s", tempo);
    else if (v->posicao > 0)   colocacao = TextFormat("Voce ficou em %do lugar com %s", v->posicao + 1, tempo);
    else                       colocacao = TextFormat("Tempo %s - fora dos 10 melhores desta vez", tempo);
    TextoCentralizado(colocacao, 95, 26, RAYWHITE);

    int x = largura / 2 - 190, y = 160;
    DrawRectangle(x - 30, y - 20, 440, 400, Fade(BLACK, 0.55f));
    if (v->posicao >= 0)
        DrawRectangle(x - 10, y + 44 + v->posicao * 30 - 4, 400, 30, Fade(GOLD, 0.35f));
    DesenharRanking(ranking, x, y);

    TextoCentralizado("Pressione ENTER para jogar uma nova campanha", altura - 60, 22, RAYWHITE);
}
