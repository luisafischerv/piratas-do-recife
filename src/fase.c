#include "fase.h"
#include <math.h>

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
        f->fundo = LoadTexture("assets/images/cenarios/fase2/mangue_fundo_01_v2.png");
        f->fundo2 = LoadTexture("assets/images/cenarios/fase2/mangue_fundo_02_v2.png");
        if (!f->fundo.id || !f->fundo2.id) {
            DescarregarFase(f);
            return false;
        }
        SetTextureFilter(f->fundo, TEXTURE_FILTER_BILINEAR);
        SetTextureFilter(f->fundo2, TEXTURE_FILTER_BILINEAR);
        f->chao = LoadTexture("assets/images/cenarios/fase2/mangue_plataforma.png");
        f->passarela = LoadTexture("assets/images/cenarios/fase2/mangue_plataforma.png");
        f->raizes = LoadTexture("assets/images/cenarios/fase2/raizes_bloqueio_proposta.png");
        f->plataformaFragil = LoadTexture("assets/images/cenarios/fase2/plataforma_fragil_proposta.png");
        if (f->chao.id) SetTextureFilter(f->chao, TEXTURE_FILTER_BILINEAR);
        if (f->passarela.id) SetTextureFilter(f->passarela, TEXTURE_FILTER_BILINEAR);
        if (f->raizes.id) SetTextureFilter(f->raizes, TEXTURE_FILTER_BILINEAR);
        if (f->plataformaFragil.id) SetTextureFilter(f->plataformaFragil, TEXTURE_FILTER_BILINEAR);
        f->largura = 2560.0f;
        f->pontoInicial = (Vector2){100.0f, 400.0f};
        f->limiteQueda = CHAO_Y + 200.0f;
        // O trecho de cipos tem espaco para tres transferencias, sem atalhos pelo chao.
        f->largura = 3280.0f;
        f->areaSaida = (Rectangle){3220, 0, 60, CHAO_Y};
        // Subir, soltar e descer: o teto impede atravessar tudo em um lancamento.
        AdicionarPlataforma(f, 0, CHAO_Y, 330, 100, PLAT_CHAO);
        AdicionarPlataforma(f, 580, 570, 90, 250, PLAT_RAIZ);
        AdicionarPlataforma(f, 790, 0, 150, 450, PLAT_RAIZ);
        AdicionarPlataforma(f, 855, CHAO_Y, 100, 20, PLAT_FRAGIL);
        // Apoios firmes permitem responder aos tiros, sem esperar sobre tabuas podres.
        AdicionarPlataforma(f, 1040, CHAO_Y, 120, 20, PLAT_FIXA);
        AdicionarPlataforma(f, 1220, 600, 100, 20, PLAT_FRAGIL);
        AdicionarPlataforma(f, 1400, CHAO_Y, 250, 100, PLAT_CHAO);
        // Nao ha apoio entre os cipos: ganhar embalo e transferir no ar.
        AdicionarPlataforma(f, 2960.0f, CHAO_Y, 320.0f, 100.0f, PLAT_CHAO);
        return true;
    }
    if (id == 3) {
        f->fundo = LoadTexture("assets/images/cenarios/fase3/escalada_navio_fundo_limpo.png");
        if (!f->fundo.id) return false;
        SetTextureFilter(f->fundo, TEXTURE_FILTER_BILINEAR);
        f->passarela = LoadTexture("assets/images/cenarios/fase2/mangue_plataforma.png");
        if (f->passarela.id) SetTextureFilter(f->passarela, TEXTURE_FILTER_BILINEAR);
        f->largura = 1280;
        f->pontoInicial = (Vector2){580, 1650};
        f->limiteQueda = 1980;
        AdicionarPlataforma(f, 530, 1800, 180, 20, PLAT_FIXA);
        return true;
    }
    return false;
}

static void DesenharDetalhe(Texture2D textura, Rectangle origem, Rectangle destino, Color cor) {
    if (textura.id) DrawTexturePro(textura, origem, destino, (Vector2){0, 0}, 0.0f, cor);
}

static void DesenharPlataformaFragil(const Fase *f, const Plataforma *p){
    Color tom = p->estado == PLAT_AVISO ? (Color){255, 130, 90, 255} : (Color){ 170, 200, 150, 255 };

    if (f->id == 2 && f->plataformaFragil.id) {
        // Recorta os postes: a superficie das tabuas coincide com o topo do colisor.
        Rectangle origem = {270, 200, 1620, 455};
        Rectangle destino = {p->area.x, p->area.y, p->area.width, 55};
        DrawTexturePro(f->plataformaFragil, origem, destino, (Vector2){0, 0}, 0, tom);
    } else if (f->passarela.id) {
        Rectangle origem = f->id == 2 ? (Rectangle){20, 155, 2130, 510} :
                                      (Rectangle){30, 220, 2110, 375};
        Rectangle destino = p->area;
        destino.height = 70.0f;
        DrawTexturePro(f->passarela, origem, destino, (Vector2){0, 0}, 0.0f, tom);
    } else {
        DrawRectangleRec(p->area, tom);
    }
    // Rachaduras permanecem visiveis antes do aviso; o aviso cresce antes da queda.
    for (int i = 1; i <= 3; i++) {
        float x = p->area.x + p->area.width * i / 4;
        DrawLineEx((Vector2){x, p->area.y}, (Vector2){x - 7, p->area.y + 13}, 3, DARKBROWN);
    }
    if (p->estado == PLAT_AVISO) {
        float progresso = 1.0f - p->tempoAviso / 0.6f;
        if (progresso > 1) progresso = 1;
        DrawRectangle((int)p->area.x, (int)p->area.y - 6,
                      (int)(p->area.width * progresso), 4, ORANGE);
    }
}

static void DesenharRaizes(Rectangle r)
{
    // Massa solida com limite legivel: desenho e colisor ocupam o mesmo retangulo.
    DrawRectangleRec(r, (Color){49, 39, 29, 255});
    for (int ramo = 0; ramo < 7; ramo++) {
        float centro = r.x + 12 + (r.width - 24) * ramo / 6;
        for (float y = 0; y < r.height; y += 8) {
            float fim = fminf(y + 8, r.height);
            float dx = sinf(y * 0.027f + ramo * 1.7f) * 8;
            float dx2 = sinf(fim * 0.027f + ramo * 1.7f) * 8;
            Vector2 a = {centro + dx, r.y + y};
            Vector2 b = {centro + dx2, r.y + fim};
            DrawLineEx(a, b, 15, (Color){42, 32, 24, 255});
            DrawLineEx(a, b, 10, (Color){104, 76, 45, 255});
            a.x -= 2; b.x -= 2;
            DrawLineEx(a, b, 2, (Color){164, 123, 68, 255});
        }
    }
    for (float y = 12; y < r.height - 12; y += 29) {
        float x = r.x + 16 + (sinf(y * 0.09f) + 1) * (r.width - 32) / 2;
        DrawEllipse((int)x, (int)(r.y + y), 11, 4, (Color){63, 97, 43, 255});
        DrawEllipse((int)x + 4, (int)(r.y + y - 4), 6, 3, (Color){112, 139, 57, 255});
    }
}

void DesenharFase(const Fase *f) {
    if (f->fundo.id) {
        Rectangle origem = {0, 0, (float)f->fundo.width, (float)f->fundo.height};
        Rectangle destino = {0, 0, 1280.0f, f->id == 3 ? 1920.0f : (float)GetScreenHeight()};
        // Escurece apenas o cenario para destacar personagens e obstaculos.
        DrawTexturePro(f->fundo, origem, destino, (Vector2){0, 0}, 0.0f,
                       (Color){165, 165, 175, 255});
    }
    if (f->fundo2.id) {
        Rectangle origem = {0, 0, (float)f->fundo2.width, (float)f->fundo2.height};
        Rectangle destino = {1280.0f, 0, f->largura - 1280.0f, (float)GetScreenHeight()};
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
        if (p->tipo == PLAT_RAIZ) {
            if (f->raizes.id) {
                // Preserva pontas e folhas da arte, que ultrapassam o colisor como decoracao.
                Rectangle origem = {0, 0, (float)f->raizes.width, (float)f->raizes.height};
                Rectangle destino = {p->area.x - p->area.width * 0.12f,
                                     p->area.y - p->area.height * 0.025f,
                                     p->area.width * 1.24f, p->area.height * 1.05f};
                DrawTexturePro(f->raizes, origem, destino, (Vector2){0, 0}, 0, WHITE);
            } else DesenharRaizes(p->area);
            continue;
        }
        if (p->tipo == PLAT_FRAGIL) {
            DesenharPlataformaFragil(f, p);
            continue;
        }

        Texture2D textura = {0};
        Rectangle origem = {0};
        Rectangle destino = p->area;
        if (p->tipo == PLAT_CHAO && f->chao.id) {
            textura = f->chao;
            origem = f->id >= 2 ? (Rectangle){20, 155, 2130, 510} :
                                  (Rectangle){0, 130, 2172, 510};
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
            origem = f->id == 2 ? (Rectangle){20, 155, 2130, 510} :
                                  (Rectangle){30, 220, 2110, 375};
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
    if (f->raizes.id) UnloadTexture(f->raizes);
    if (f->plataformaFragil.id) UnloadTexture(f->plataformaFragil);
    f->fundo.id = 0;
    f->fundo2.id = 0;
    f->chao.id = f->caixa.id = f->passarela.id = 0;
    f->cargas.id = 0;
    f->raizes.id = f->plataformaFragil.id = 0;
}
