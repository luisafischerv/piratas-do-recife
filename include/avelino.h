#ifndef AVELINO_H
#define AVELINO_H
#include "combate.h"
#include "tiros.h"
#define VIDA_AVELINO 20

typedef struct Avelino {
    Texture2D golpe;
    Texture2D andar;
    float tempo;
    float tempoAndar;
    float x;
    int direcao;
    int saude;
    int ultimoGolpe;
    bool acertou;
    bool caminhando;
} Avelino;

void CarregarAvelino(Avelino *a);
void ReiniciarAvelino(Avelino *a);
int EstagioAvelino(const Avelino *a);
void DescarregarAvelino(Avelino *a);
int AtualizarAvelino(Avelino *a, const Jogador *j, const Espada *espada, SistemaTiros *tiros, float dt);
void DesenharAvelino(const Avelino *a);
#endif
