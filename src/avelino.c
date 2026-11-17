#include "avelino.h"
#include <math.h>

#define LARGURA_IMPACTO 300.0f
#define ALCANCE_ATRAS 60.0f

static Rectangle AreaImpacto(const Avelino *a) {
    float x = a->direcao < 0 ? a->x - 130.0f : a->x + 260.0f - ALCANCE_ATRAS;
    return (Rectangle){x, 530, LARGURA_IMPACTO + ALCANCE_ATRAS, 70};
}

int EstagioAvelino(const Avelino *a) {
    return a->saude > 14 ? 1 : a->saude > 7 ? 2 : 3;
}

void ReiniciarAvelino(Avelino *a) { a->tempo = 0; a->tempoAndar = 0; a->caminhando = false; a->x = 830; a->direcao = -1; a->saude = VIDA_AVELINO; a->ultimoGolpe = 0; a->acertou = false; }
void CarregarAvelino(Avelino *a) {
    *a = (Avelino){0};
    a->golpe = LoadTexture("assets/images/avelino/avelino_golpe.png");
    a->andar = LoadTexture("assets/images/avelino/avelino_andar.png");
    if (a->golpe.id) SetTextureFilter(a->golpe, TEXTURE_FILTER_BILINEAR);
    if (a->andar.id) SetTextureFilter(a->andar, TEXTURE_FILTER_BILINEAR);
    ReiniciarAvelino(a);
}
void DescarregarAvelino(Avelino *a) { if (a->golpe.id) UnloadTexture(a->golpe); if (a->andar.id) UnloadTexture(a->andar); }

int AtualizarAvelino(Avelino *a, const Jogador *j, const Espada *espada, SistemaTiros *tiros, float dt) {
    if (a->saude <= 0) return 0;
    int estagio = EstagioAvelino(a);
    a->caminhando = false;
    // Espera ate estar perto antes de iniciar a preparacao do golpe.
    float distancia = j->corpo.x + j->corpo.width / 2 - (a->x + 215);
    if (a->tempo < 0.8f && fabsf(distancia) > 300.0f) {
        a->direcao = distancia < 0 ? -1 : 1;
        float xAnterior = a->x;
        a->x += a->direcao * (165.0f + 50.0f * (estagio - 1)) * dt;
        if (a->x < 0) a->x = 0;
        if (a->x > 850) a->x = 850;
        a->caminhando = a->x != xAnterior;
        if (a->caminhando) a->tempoAndar += dt;
    } else {
        if (a->tempo < 0.8f && fabsf(distancia) > 1.0f)
            a->direcao = distancia < 0 ? -1 : 1;
        a->tempo += dt * (1.0f + 0.12f * (estagio - 1));
    }
    float ciclo = estagio == 1 ? 5.0f : estagio == 2 ? 4.6f : 4.2f;
    if (a->tempo >= ciclo) { a->tempo -= ciclo; a->acertou = false; }
    Rectangle impacto = AreaImpacto(a);
    Rectangle mao = {a->direcao < 0 ? a->x - 90 : a->x + 300, 480, 160, 120};
    int dano = 0;
    if (a->tempo >= 2.1f && a->tempo < 2.45f && !a->acertou && CheckCollisionRecs(impacto, j->corpo)) {
        a->acertou = true;
        dano = 1;
    }
    if (a->tempo >= 2.1f && a->tempo < 3.7f) {
        if (espada->ativa && espada->idGolpe != a->ultimoGolpe && CheckCollisionRecs(AreaEspada(j), mao)) {
            a->ultimoGolpe = espada->idGolpe;
            a->saude--;
        }
        for (int i = 0; i < MAX_TIROS; i++) {
            Tiro *t = &tiros->tiros[i];
            Rectangle percurso = t->area;
            float deslocamento = t->velocidade * dt;
            if (deslocamento < 0) percurso.x += deslocamento;
            percurso.width += deslocamento < 0 ? -deslocamento : deslocamento;
            if (t->ativo && t->doJogador && CheckCollisionRecs(percurso, mao)) {
                t->ativo = false;
                a->saude--;
            }
        }
    }
    if (a->saude < 0) a->saude = 0;
    return dano;
}

void DesenharAvelino(const Avelino *a) {
    if (!a->golpe.id) return;
    float t = a->tempo;
    int quadro = t < 0.8f ? 0 : t < 1.15f ? 1 : t < 1.5f ? 2 : t < 1.95f ? 3 : t < 2.1f ? 4 : t < 2.6f ? 5 : t < 3.7f ? 6 : 7;
    if (a->saude <= 0) quadro = 7;
    Texture2D textura = a->caminhando && a->andar.id ? a->andar : a->golpe;
    if (textura.id == a->andar.id && a->caminhando) quadro = (int)(a->tempoAndar * 7.0f) % 8;
    float w = textura.width / 4.0f, h = textura.height / 2.0f;
    if (a->saude > 0 && t >= 0.8f && t < 2.1f)
        DrawRectangleRec((Rectangle){AreaImpacto(a).x, 590, AreaImpacto(a).width, 10}, Fade(GRAY, 0.75f));
    Rectangle origem = {(quadro % 4)*w, (quadro / 4)*h, a->direcao < 0 ? w : -w, h};
    DrawTexturePro(textura, origem, (Rectangle){a->x, 164, 430, 443}, (Vector2){0, 0}, 0, WHITE);
    DrawRectangle(770, 70, 440, 18, DARKGRAY);
    DrawRectangle(770, 70, 440 * a->saude / VIDA_AVELINO, 18, MAROON);
    DrawText("CAPITAO AVELINO", 770, 45, 20, RAYWHITE);
}
