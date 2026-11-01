#include "jogador.h"

#define VELOCIDADE_HORIZONTAL 260.0f
#define FORCA_SALTO            520.0f
#define GRAVIDADE              1300.0f

void InicializarJogador(Jogador *jogador)
{
    jogador->corpo = (Rectangle){ 100.0f, 400.0f, 48.0f, 64.0f };
    jogador->velocidadeY = 0.0f;
    jogador->estaNoChao = false;
    jogador->direcao = 1;
}

void AtualizarJogador(Jogador *jogador, float dt)
{
    float movimentoX = 0.0f;

    if (IsKeyDown(KEY_A)) movimentoX -= 1.0f;
    if (IsKeyDown(KEY_D)) movimentoX += 1.0f;

    jogador->corpo.x += movimentoX * VELOCIDADE_HORIZONTAL * dt;

    if (movimentoX < 0.0f) jogador->direcao = -1;
    if (movimentoX > 0.0f) jogador->direcao = 1;

    if (IsKeyPressed(KEY_SPACE) && jogador->estaNoChao) {
        jogador->velocidadeY = -FORCA_SALTO;
        jogador->estaNoChao = false;
    }

    
    jogador->velocidadeY += GRAVIDADE * dt;
    jogador->corpo.y += jogador->velocidadeY * dt;

    const float chaoY = (float)GetScreenHeight() - 100.0f;
    if (jogador->corpo.y + jogador->corpo.height >= chaoY) {
        jogador->corpo.y = chaoY - jogador->corpo.height;
        jogador->velocidadeY = 0.0f;
        jogador->estaNoChao = true;
    }

    if (jogador->corpo.x < 0.0f) jogador->corpo.x = 0.0f;
    if (jogador->corpo.x + jogador->corpo.width > GetScreenWidth()) {
        jogador->corpo.x = (float)GetScreenWidth() - jogador->corpo.width;
    }
}

void DesenharJogador(const Jogador *jogador)
{
    DrawRectangleRec(jogador->corpo, BLUE);
}
