#ifndef MARUJO_H
#define MARUJO_H
#define MARUJO_SAUDE 3

#include "inimigos.h"
#include "jogador.h"

Inimigo CriarMarujo(float x, float chaoY, float limiteEsquerdo, float limiteDireito);
Rectangle AreaGolpeMarujo(const Inimigo *inimigo);
int AtualizarMarujo(Inimigo *inimigo, const Jogador *jogador, float dt);

// Quantos marujos vivos existem na lista agora.
int ContarMarujos(const ListaInimigos *lista);

// Convocacao do chefe (estagios 2 e 3): insere ate 'quantidade' marujos na arena,
// sem deixar passar de 'limite' marujos vivos ao mesmo tempo.
// Eles surgem no lado da arena mais longe do Nico, virados para ele.
// Retorna quantos foram inseridos (0 se a arena ja esta no limite).
// Na derrota no chefe, LiberarInimigos limpa os convocados.
int ConvocarMarujos(ListaInimigos *lista, int quantidade, int limite,
                    float chaoY, float limiteEsquerdo, float limiteDireito,
                    float xJogador);

#endif