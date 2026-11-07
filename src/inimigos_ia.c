#include "inimigos_ia.h"
#include "marujo.h"

bool RetangulosSobrepostos(Rectangle a, Rectangle b)
{
    return a.x < b.x + b.width && a.x + a.width > b.x &&
           a.y < b.y + b.height && a.y + a.height > b.y;
}

float CentroXRetangulo(Rectangle r) { return r.x + r.width / 2.0f; }
float CentroYRetangulo(Rectangle r) { return r.y + r.height / 2.0f; }

void MoverInimigoNosLimites(Inimigo *inimigo, float deslocamento)
{
    inimigo->corpo.x += deslocamento;
    if (inimigo->corpo.x < inimigo->limiteEsquerdo)
        inimigo->corpo.x = inimigo->limiteEsquerdo;
    if (inimigo->corpo.x + inimigo->corpo.width > inimigo->limiteDireito)
        inimigo->corpo.x = inimigo->limiteDireito - inimigo->corpo.width;
}

void MudarEstadoInimigo(Inimigo *inimigo, EstadoInimigo novo, float duracao)
{
    inimigo->estado = novo;
    inimigo->timerEstado = duracao;
}

int AtualizarInimigos(ListaInimigos *lista, const Jogador *jogador, float dt)
{
    int danoTotal = 0;
    NoInimigo *atual = lista->inicio;
    while (atual != NULL) {
        NoInimigo *proximo = atual->proximo;
        Inimigo *inimigo = &atual->dados;
        if (inimigo->saude <= 0) {
            RemoverInimigo(lista, atual);
        } else {
            switch (inimigo->tipo) {
            case INIMIGO_MARUJO:
                danoTotal += AtualizarMarujo(inimigo, jogador, dt);
                break;
            case INIMIGO_ATIRADOR:     // TODO: AtualizarAtirador (Pessoa 1)
            case INIMIGO_CARANGUEJO:   // TODO: AtualizarCaranguejo (Pessoa 2)
                break;
            }
        }
        atual = proximo;
    }

    return danoTotal;
}