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
    jogador->texturaCorrida = LoadTexture("assets/images/nico/nico_corrida_sheet.png");
    jogador->texturaParado = LoadTexture("assets/images/nico/nico_parado_12_sheet.png");
    jogador->texturaSalto = LoadTexture("assets/images/nico/nico_salto.png");
    jogador->tempoAnimacao = 0.0f;
    jogador->quadroAtual = 0;
    jogador->andando = false;
    if (jogador->texturaCorrida.id) SetTextureFilter(jogador->texturaCorrida, TEXTURE_FILTER_BILINEAR);
    if (jogador->texturaParado.id) SetTextureFilter(jogador->texturaParado, TEXTURE_FILTER_BILINEAR);
    if (jogador->texturaSalto.id) SetTextureFilter(jogador->texturaSalto, TEXTURE_FILTER_BILINEAR);
}

void AtualizarJogador(Jogador *jogador, float dt)
{
    bool estavaAndando = jogador->andando;
    bool estavaNoChao = jogador->estaNoChao;
    float movimentoX = 0.0f;

    if (IsKeyDown(KEY_A)) movimentoX -= 1.0f;
    if (IsKeyDown(KEY_D)) movimentoX += 1.0f;

    jogador->andando = movimentoX != 0.0f;
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
    // Cada estado comeca no primeiro quadro, com seu proprio ritmo.
    if (estavaAndando != jogador->andando || estavaNoChao != jogador->estaNoChao) {
        jogador->tempoAnimacao = 0.0f;
        jogador->quadroAtual = 0;
    }
    if (jogador->estaNoChao) {
        // Repouso: 12 quadros de 0,2 s, totalizando 2,4 s por ciclo.
        float intervalo = jogador->andando ? 0.1f : 0.2f;
        int totalQuadros = jogador->andando ? 8 : 12;
        jogador->tempoAnimacao += dt;
        while (jogador->tempoAnimacao >= intervalo) {
            jogador->tempoAnimacao -= intervalo;
            jogador->quadroAtual = (jogador->quadroAtual + 1) % totalQuadros;
        }
    } else {
        jogador->tempoAnimacao = 0.0f;
        jogador->quadroAtual = 0;
    }
}

void DescarregarJogador(Jogador *jogador)
{
    if (jogador->texturaCorrida.id) UnloadTexture(jogador->texturaCorrida);
    if (jogador->texturaParado.id) UnloadTexture(jogador->texturaParado);
    if (jogador->texturaSalto.id) UnloadTexture(jogador->texturaSalto);
    jogador->texturaCorrida.id = jogador->texturaParado.id = jogador->texturaSalto.id = 0;
}

void DesenharJogador(const Jogador *jogador)
{
    bool correndo = jogador->andando && jogador->estaNoChao;
    Texture2D textura = !jogador->estaNoChao ? jogador->texturaSalto :
        (correndo ? jogador->texturaCorrida : jogador->texturaParado);
    if (!textura.id) {
        DrawRectangleRec(jogador->corpo, BLUE);
        return;
    }
    float largura = jogador->estaNoChao ? textura.width / 4.0f : (float)textura.width;
    float altura = correndo ? textura.height / 2.0f :
        (jogador->estaNoChao ? textura.height / 3.0f : (float)textura.height);
    int quadro = jogador->estaNoChao ? jogador->quadroAtual : 0;
    Rectangle origem = { (quadro % 4) * largura, (quadro / 4) * altura,
                         largura * jogador->direcao, altura };
    // A arte e maior que o colisor; os pes ficam ancorados na base do corpo.
    float alturaVisual = correndo ? 120.0f : (jogador->estaNoChao ? 128.0f : 168.0f);
    float larguraVisual = alturaVisual * largura / altura;
    float ancoraY = correndo ? 0.98f : (jogador->estaNoChao ? 0.94f : 736.0f / 768.0f);
    Rectangle destino = { jogador->corpo.x + jogador->corpo.width / 2.0f - larguraVisual / 2.0f,
        jogador->corpo.y + jogador->corpo.height - alturaVisual * ancoraY,
        larguraVisual, alturaVisual };
    DrawTexturePro(textura, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
}
