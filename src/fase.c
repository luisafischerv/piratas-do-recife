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
        f->largura = 1280.0f;
        AdicionarPlataforma(f, 0.0f, CHAO_Y, 1280.0f, 100.0f, PLAT_CHAO);
        AdicionarPlataforma(f, 300.0f, CHAO_Y - 60.0f, 60.0f, 60.0f, PLAT_CAIXA);
        AdicionarPlataforma(f, 860.0f, CHAO_Y - 60.0f, 60.0f, 60.0f, PLAT_CAIXA);
        AdicionarPlataforma(f, 400.0f, CHAO_Y - 140.0f, 200.0f, 20.0f, PLAT_FIXA);
        AdicionarPlataforma(f, 960.0f, CHAO_Y - 140.0f, 180.0f, 20.0f, PLAT_FIXA);
        return true;
    }
    return false;
}

void DesenharFase(const Fase *f){
    for (int i = 0; i < f->qtdPlataformas; i++) {
        const Plataforma *p = &f->plataformas[i];
        if (p->ativa) DrawRectangleRec(p->area, (Color){ 92, 84, 78, 255 });
    }
}