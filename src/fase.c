#include "fase.h"

#define CHAO_Y 620.0f

static void AdicionarPlataforma(Fase *f, float x, float y, float w, float h, TipoPlataforma tipo){
    if (f->qtdPlataformas >= MAX_PLATAFORMAS) return;
    Plataforma *p = &f->plataformas[f->qtdPlataformas];
    p->area = (Rectangle){ x, y, w, h };
    p->tipo = tipo;
    p->estado = PLAT_NORMAL;
    p->tempoAviso = 0.0f;
    p->ativa = true;
    f->qtdPlataformas++;
}

bool CarregarFase(Fase *f, int id){
    *f = (Fase){0};
    f->id = id;

    if (id == 1) {
        f->fundo = LoadTexture("assets/images/cenarios/cais_fundo.png");
        f->fundo2 = LoadTexture("assets/images/cenarios/cais_fundo_02.png");
        f->chao = LoadTexture("assets/images/cenarios/cais_chao.png");
        f->caixa = LoadTexture("assets/images/cenarios/cais_caixa.png");
        f->passarela = LoadTexture("assets/images/cenarios/cais_passarela.png");
        f->cargas = LoadTexture("assets/images/cenarios/propostas/cargas.png");
        if (f->fundo.id) SetTextureFilter(f->fundo, TEXTURE_FILTER_BILINEAR);
        if (f->fundo2.id) SetTextureFilter(f->fundo2, TEXTURE_FILTER_BILINEAR);
        if (f->chao.id) SetTextureFilter(f->chao, TEXTURE_FILTER_BILINEAR);
        if (f->caixa.id) SetTextureFilter(f->caixa, TEXTURE_FILTER_BILINEAR);
        if (f->passarela.id) SetTextureFilter(f->passarela, TEXTURE_FILTER_BILINEAR);
        if (f->cargas.id) SetTextureFilter(f->cargas, TEXTURE_FILTER_BILINEAR);
        f->largura = 2560.0f;
        f->limiteQueda = CHAO_Y + 200.0f;
        f->pontoInicial = (Vector2){100.0f, 400.0f};
        f->areaSaida = (Rectangle){2500.0f, 0.0f, 60.0f, CHAO_Y};
        AdicionarPlataforma(f, 0.0f, CHAO_Y, 1280.0f, 100.0f, PLAT_CHAO);
        AdicionarPlataforma(f, 1280.0f, CHAO_Y, 1280.0f, 100.0f, PLAT_CHAO);
        // Primeiro trecho: caixa de aprendizado e acesso a rota elevada opcional.
        AdicionarPlataforma(f, 235.0f, CHAO_Y - 48.0f, 42.0f, 48.0f, PLAT_BARRIL);
        AdicionarPlataforma(f, 340.0f, CHAO_Y - 60.0f, 60.0f, 60.0f, PLAT_CAIXA);
        AdicionarPlataforma(f, 430.0f, CHAO_Y - 140.0f, 230.0f, 20.0f, PLAT_FIXA);
        // A rota de cima alterna pequenas subidas, descidas e espacamentos.
        AdicionarPlataforma(f, 750.0f, CHAO_Y - 190.0f, 180.0f, 20.0f, PLAT_FIXA);
        AdicionarPlataforma(f, 1010.0f, CHAO_Y - 140.0f, 180.0f, 20.0f, PLAT_FIXA);
        // Espaco aberto para combate na transicao dos dois trechos.
        // Na segunda area, caixas em degraus levam a uma passarela maior.
        AdicionarPlataforma(f, 1420.0f, CHAO_Y - 40.0f, 40.0f, 40.0f, PLAT_CAIXA);
        AdicionarPlataforma(f, 1462.0f, CHAO_Y - 30.0f, 44.0f, 30.0f, PLAT_SACO);
        AdicionarPlataforma(f, 1510.0f, CHAO_Y - 60.0f, 60.0f, 60.0f, PLAT_CAIXA);
        AdicionarPlataforma(f, 1600.0f, CHAO_Y - 140.0f, 260.0f, 20.0f, PLAT_FIXA);
        AdicionarPlataforma(f, 1950.0f, CHAO_Y - 185.0f, 170.0f, 20.0f, PLAT_FIXA);
        AdicionarPlataforma(f, 2050.0f, CHAO_Y - 60.0f, 60.0f, 60.0f, PLAT_CAIXA);
        return true;
    }
    if (id == 2) {
        f->fundo = LoadTexture("assets/images/cenarios/fase2/mangue_fundo_01.png");
        f->fundo2 = LoadTexture("assets/images/cenarios/fase2/mangue_fundo_02.png");
        if (!f->fundo.id || !f->fundo2.id) {
            DescarregarFase(f);
            return false;
        }
        SetTextureFilter(f->fundo, TEXTURE_FILTER_BILINEAR);
        SetTextureFilter(f->fundo2, TEXTURE_FILTER_BILINEAR);
        // Reutilizados provisoriamente ate criarmos as pecas do mangue.
        f->chao = LoadTexture("assets/images/cenarios/cais_chao.png");
        f->passarela = LoadTexture("assets/images/cenarios/cais_passarela.png");
        if (f->chao.id) SetTextureFilter(f->chao, TEXTURE_FILTER_BILINEAR);
        if (f->passarela.id) SetTextureFilter(f->passarela, TEXTURE_FILTER_BILINEAR);
        f->largura = 2560.0f;
        f->pontoInicial = (Vector2){100.0f, 400.0f};
        f->limiteQueda = CHAO_Y + 200.0f;
        AdicionarPlataforma(f, 0.0f, CHAO_Y, 1280.0f, 100.0f, PLAT_CHAO);
        AdicionarPlataforma(f, 1280.0f, CHAO_Y, 370.0f, 100.0f, PLAT_CHAO);
        AdicionarPlataforma(f, 1650.0f, CHAO_Y, 130.0f, 20.0f, PLAT_FRAGIL);
        AdicionarPlataforma(f, 1830.0f, CHAO_Y, 130.0f, 20.0f, PLAT_FRAGIL);
        AdicionarPlataforma(f, 2010.0f, CHAO_Y, 130.0f, 20.0f, PLAT_FRAGIL);
        AdicionarPlataforma(f, 2190.0f, CHAO_Y, 100.0f, 20.0f, PLAT_FRAGIL);
        AdicionarPlataforma(f, 2290.0f, CHAO_Y, 270.0f, 100.0f, PLAT_CHAO);
        return true;
    }
    return false;
}

static void DesenharDetalhe(Texture2D textura, Rectangle origem, Rectangle destino, Color cor) {
    if (textura.id) DrawTexturePro(textura, origem, destino, (Vector2){0, 0}, 0.0f, cor);
}

static void DesenharPlataformaFragil(const Fase *f, const Plataforma *p){
    Color tom = (Color){ 170, 200, 150, 255 };

    if (f->passarela.id) {
        Rectangle origem = { 30, 220, 2110, 375 };
        Rectangle destino = p->area;
        destino.height = 70.0f;
        DrawTexturePro(f->passarela, origem, destino, (Vector2){0, 0}, 0.0f, tom);
    } else {
        DrawRectangleRec(p->area, tom);
    }
}

void DesenharFase(const Fase *f) {
    if (f->fundo.id) {
        Rectangle origem = {0, 0, (float)f->fundo.width, (float)f->fundo.height};
        Rectangle destino = {0, 0, 1280.0f, (float)GetScreenHeight()};
        // Escurece apenas o cenario para destacar personagens e obstaculos.
        DrawTexturePro(f->fundo, origem, destino, (Vector2){0, 0}, 0.0f,
                       (Color){165, 165, 175, 255});
    }
    if (f->fundo2.id) {
        Rectangle origem = {0, 0, (float)f->fundo2.width, (float)f->fundo2.height};
        Rectangle destino = {1280.0f, 0, 1280.0f, (float)GetScreenHeight()};
        DrawTexturePro(f->fundo2, origem, destino, (Vector2){0, 0}, 0.0f,
                       (Color){165, 165, 175, 255});
    }
    // Elementos de fundo sem colisao, atras de plataformas e personagens.
    Color decoracao = (Color){185, 185, 195, 255};
    DesenharDetalhe(f->cargas, (Rectangle){650, 730, 560, 400},
                    (Rectangle){1160, 585, 62, 35}, decoracao);
    for (int i = 0; i < f->qtdPlataformas; i++) {
        const Plataforma *p = &f->plataformas[i];
        if (!p->ativa) continue;
        if (p->tipo == PLAT_FRAGIL) {
            DesenharPlataformaFragil(f, p);
            continue;
        }

        Texture2D textura = {0};
        Rectangle origem = {0};
        Rectangle destino = p->area;
        if (p->tipo == PLAT_CHAO && f->chao.id) {
            textura = f->chao;
            origem = (Rectangle){0, 130, 2172, 510};
        } else if (p->tipo == PLAT_CAIXA && f->caixa.id) {
            textura = f->caixa;
            origem = (Rectangle){48, 55, 1160, 1160};
        } else if (p->tipo == PLAT_BARRIL && f->cargas.id) {
            textura = f->cargas;
            origem = (Rectangle){140, 65, 390, 490};
        } else if (p->tipo == PLAT_SACO && f->cargas.id) {
            textura = f->cargas;
            origem = (Rectangle){110, 620, 450, 510};
        } else if (p->tipo == PLAT_FIXA && f->passarela.id) {
            textura = f->passarela;
            origem = (Rectangle){30, 220, 2110, 375};
            // A parte superior coincide com o colisor; suportes sao decorativos.
            destino.height = 70.0f;
        }
        if (textura.id) {
            DrawTexturePro(textura, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
            continue;
        }
        Color cor = (Color){ 92, 84, 78, 255 };
        if (p->tipo == PLAT_CAIXA) cor = (Color){ 150, 98, 52, 255 };
        if (p->tipo == PLAT_FIXA)  cor = (Color){ 176, 124, 70, 255 };

        DrawRectangleRec(p->area, cor);
        DrawRectangleLinesEx(p->area, 2.0f, (Color){ 60, 40, 24, 255 });
    }
}

void DescarregarFase(Fase *f) {
    if (f->fundo.id) UnloadTexture(f->fundo);
    if (f->fundo2.id) UnloadTexture(f->fundo2);
    if (f->chao.id) UnloadTexture(f->chao);
    if (f->caixa.id) UnloadTexture(f->caixa);
    if (f->passarela.id) UnloadTexture(f->passarela);
    if (f->cargas.id) UnloadTexture(f->cargas);
    f->fundo.id = 0;
    f->fundo2.id = 0;
    f->chao.id = f->caixa.id = f->passarela.id = 0;
    f->cargas.id = 0;
}
