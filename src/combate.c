#include "combate.h"
#include "inimigos_ia.h"

// Valores EXPERIMENTAIS
#define ESPADA_TEMPO_ATIVO   0.15f  // segundos em que a area causa dano
#define ESPADA_TEMPO_RECARGA 0.30f  // intervalo minimo entre dois golpes
#define ESPADA_ALCANCE       70.0f  // largura da area a frente do Nico
#define ESPADA_DANO          1
#define TEMPO_PISCA_DANO     0.15f  // inimigo pisca branco depois do acerto

void InicializarEspada(Espada *espada)
{
    espada->ativa = false;
    espada->tempoAtivo = 0.0f;
    espada->tempoRecarga = 0.0f;
    espada->idGolpe = 0;
}

bool AtualizarAtaqueEspada(Espada *espada, bool clicou, float dt)
{
    if (espada->tempoRecarga > 0.0f) espada->tempoRecarga -= dt;

    if (espada->ativa) {
        espada->tempoAtivo -= dt;
        if (espada->tempoAtivo <= 0.0f) espada->ativa = false;
    }

    // Clique durante a recarga e ignorado (nao fica guardado para depois).
    if (clicou && espada->tempoRecarga <= 0.0f) {
        espada->ativa = true;
        espada->tempoAtivo = ESPADA_TEMPO_ATIVO;
        espada->tempoRecarga = ESPADA_TEMPO_RECARGA;
        espada->idGolpe++;
        return true;
    }
    return false;
}

Rectangle AreaEspada(const Jogador *jogador)
{
    const Rectangle c = jogador->corpo;
    float altura = c.height * 0.7f;
    float y = c.y + c.height * 0.15f;
    float x = (jogador->direcao > 0) ? c.x + c.width : c.x - ESPADA_ALCANCE;
    return (Rectangle){ x, y, ESPADA_ALCANCE, altura };
}

int VerificarAcertosEspada(const Espada *espada, const Jogador *jogador, ListaInimigos *lista)
{
    if (!espada->ativa) return 0;

    Rectangle area = AreaEspada(jogador);
    int atingidos = 0;

    for (NoInimigo *no = lista->inicio; no != NULL; no = no->proximo) {
        Inimigo *inimigo = &no->dados;
        if (inimigo->saude <= 0) continue;                              // ja morreu
        if (inimigo->ultimoGolpeRecebido == espada->idGolpe) continue;  // ja levou ESTE golpe
        if (!RetangulosSobrepostos(area, inimigo->corpo)) continue;     // fora do alcance

        inimigo->saude -= ESPADA_DANO;
        inimigo->ultimoGolpeRecebido = espada->idGolpe;
        inimigo->tempoDano = TEMPO_PISCA_DANO;
        atingidos++;
    }
    return atingidos;
}
