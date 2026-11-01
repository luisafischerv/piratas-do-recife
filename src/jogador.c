#include "jogador.h"

#define VELOCIDADE_HORIZONTAL 260.0f
#define FORCA_SALTO            520.0f
#define GRAVIDADE              1300.0f

// Dimensoes de desenho, independentes do retangulo de colisao.
// Corrida e repouso crescem juntos para conservar a proporcao ja aprovada.
#define ALTURA_VISUAL_SALTO    140.0f
#define ESCALA_VISUAL_CHAO     (140.0f / 120.0f)
#define ALTURA_VISUAL_CORRIDA  (120.0f * ESCALA_VISUAL_CHAO)
#define ALTURA_VISUAL_REPOUSO  (128.0f * ESCALA_VISUAL_CHAO)

void InicializarJogador(Jogador *jogador)
{
    jogador->corpo = (Rectangle){ 100.0f, 400.0f, 48.0f, 64.0f };
    jogador->velocidadeY = 0.0f;
    jogador->estaNoChao = false;
    jogador->direcao = 1;
    jogador->texturaCorrida = LoadTexture("assets/images/nico/nico_corrida_sheet.png");
    jogador->texturaParado = LoadTexture("assets/images/nico/nico_parado_12_sheet.png");
    jogador->texturaSalto = LoadTexture("assets/images/nico/nico_salto_vertical_sheet.png");
    jogador->texturaSaltoDiagonal = LoadTexture("assets/images/nico/nico_salto_diagonal_sheet.png");
    jogador->saltoDiagonal = false;
    jogador->preparoSalto = 0.0f;
    jogador->tempoNoAr = 0.0f;
    jogador->tempoPouso = 0.0f;
    jogador->tempoAnimacao = 0.0f;
    jogador->quadroAtual = 0;
    jogador->andando = false;
    if (jogador->texturaCorrida.id) SetTextureFilter(jogador->texturaCorrida, TEXTURE_FILTER_BILINEAR);
    if (jogador->texturaParado.id) SetTextureFilter(jogador->texturaParado, TEXTURE_FILTER_BILINEAR);
    if (jogador->texturaSalto.id) SetTextureFilter(jogador->texturaSalto, TEXTURE_FILTER_BILINEAR);
    if (jogador->texturaSaltoDiagonal.id) SetTextureFilter(jogador->texturaSaltoDiagonal, TEXTURE_FILTER_BILINEAR);
}

void AtualizarJogador(Jogador *jogador, float dt)
{
    bool estavaAndando = jogador->andando;
    bool estavaNoChao = jogador->estaNoChao;
    float movimentoX = 0.0f;
    if (jogador->tempoPouso > 0.0f) jogador->tempoPouso -= dt;
    if (jogador->preparoSalto > 0.0f) {
        jogador->preparoSalto -= dt;
        if (jogador->preparoSalto <= 0.0f && jogador->estaNoChao) {
            jogador->velocidadeY = -FORCA_SALTO;
            jogador->estaNoChao = false;
            jogador->tempoNoAr = 0.0f;
        }
    }

    if (IsKeyDown(KEY_A)) movimentoX -= 1.0f;
    if (IsKeyDown(KEY_D)) movimentoX += 1.0f;

    jogador->andando = movimentoX != 0.0f;
    jogador->corpo.x += movimentoX * VELOCIDADE_HORIZONTAL * dt;

    if (movimentoX < 0.0f) jogador->direcao = -1;
    if (movimentoX > 0.0f) jogador->direcao = 1;

    if (IsKeyPressed(KEY_SPACE) && jogador->estaNoChao && jogador->preparoSalto <= 0.0f) {
        // Guarda a variante ate o pouso, evitando trocar de folha no ar.
        jogador->saltoDiagonal = jogador->andando;
        jogador->preparoSalto = 0.04f;
        jogador->tempoPouso = 0.0f;
    }

    
    jogador->velocidadeY += GRAVIDADE * dt;
    jogador->corpo.y += jogador->velocidadeY * dt;

    const float chaoY = (float)GetScreenHeight() - 100.0f;
    bool estavaNoAr = !jogador->estaNoChao;
    if (estavaNoAr) jogador->tempoNoAr += dt;
    if (jogador->corpo.y + jogador->corpo.height >= chaoY) {
        jogador->corpo.y = chaoY - jogador->corpo.height;
        jogador->velocidadeY = 0.0f;
        jogador->estaNoChao = true;
        if (estavaNoAr) jogador->tempoPouso = 0.1f;
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
    if (jogador->texturaSaltoDiagonal.id) UnloadTexture(jogador->texturaSaltoDiagonal);
    jogador->texturaSaltoDiagonal.id = 0;
    jogador->texturaCorrida.id = jogador->texturaParado.id = jogador->texturaSalto.id = 0;
}

void DesenharJogador(const Jogador *jogador)
{
    bool saltando = !jogador->estaNoChao || jogador->preparoSalto > 0.0f || jogador->tempoPouso > 0.0f;
    bool correndo = jogador->andando && jogador->estaNoChao;
    Texture2D texturaSalto = jogador->saltoDiagonal && jogador->texturaSaltoDiagonal.id ?
        jogador->texturaSaltoDiagonal : jogador->texturaSalto;
    Texture2D textura = saltando ? texturaSalto :
        (correndo ? jogador->texturaCorrida : jogador->texturaParado);
    if (!textura.id) {
        DrawRectangleRec(jogador->corpo, BLUE);
        return;
    }
    int colunas = saltando ? 3 : 4;
    int linhas = saltando || correndo ? 2 : 3;
    float largura = (float)textura.width / colunas;
    float altura = (float)textura.height / linhas;
    int quadro = jogador->quadroAtual;
    if (saltando) {
        if (jogador->preparoSalto > 0.0f) quadro = 0;
        else if (jogador->estaNoChao) quadro = 5;
        else if (jogador->tempoNoAr < 0.08f && jogador->velocidadeY < 0.0f) quadro = 1;
        else if (jogador->velocidadeY < -80.0f) quadro = 2;
        else if (jogador->velocidadeY <= 80.0f) quadro = 3;
        else quadro = 4;
    }
    Rectangle origem = { (quadro % colunas) * largura, (quadro / colunas) * altura,
                         largura * jogador->direcao, altura };
    // A arte e maior que o colisor; os pes ficam ancorados na base do corpo.
    float alturaVisual = saltando ? ALTURA_VISUAL_SALTO :
        (correndo ? ALTURA_VISUAL_CORRIDA : ALTURA_VISUAL_REPOUSO);
    float larguraVisual = alturaVisual * largura / altura;
    // Subida e apice usam a mesma referencia vertical do quadril.
    // A troca de pose nao deve acrescentar um salto a trajetoria da fisica.
    const float ancorasSalto[6] = {0.96f, 0.88f, 0.82f, 0.82f, 0.88f, 0.94f};
    // Centro do quadril em cada celula de 512 px, independente dos bracos.
    const float centrosSalto[6] = {258.0f, 255.0f, 250.0f, 259.0f, 257.0f, 255.0f};
    const float ancorasDiagonal[6] = {0.94f, 0.83f, 0.81f, 0.81f, 0.85f, 0.96f};
    const float centrosDiagonal[6] = {250.0f, 258.0f, 254.0f, 263.0f, 265.0f, 259.0f};
    bool diagonal = saltando && jogador->saltoDiagonal && jogador->texturaSaltoDiagonal.id;
    float ancoraY = saltando ? (diagonal ? ancorasDiagonal[quadro] : ancorasSalto[quadro]) : (correndo ? 0.98f : 0.94f);
    float ancoraX = saltando ? (diagonal ? centrosDiagonal[quadro] : centrosSalto[quadro]) / 512.0f : 0.5f;
    if (jogador->direcao < 0) ancoraX = 1.0f - ancoraX;
    Rectangle destino = { jogador->corpo.x + jogador->corpo.width / 2.0f - larguraVisual * ancoraX,
        jogador->corpo.y + jogador->corpo.height - alturaVisual * ancoraY,
        larguraVisual, alturaVisual };
    DrawTexturePro(textura, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
}
