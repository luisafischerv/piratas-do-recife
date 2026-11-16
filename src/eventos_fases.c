#include "eventos.h"

#define CHAO_Y 620.0f   // mesmo valor do CHAO_Y da fase.c (Pessoa 2)

bool CadastrarSurgimentoMarujo(SequenciaEventos *seq, float instante, float x, float chaoY,
                               float limiteEsquerdo, float limiteDireito)
{
    Evento e = {0};
    e.instante = instante;
    e.tipo = EVENTO_SURGIR_MARUJO;
    e.x = x;
    e.chaoY = chaoY;
    e.limiteEsquerdo = limiteEsquerdo;
    e.limiteDireito = limiteDireito;
    return CadastrarEvento(seq, e);
}

bool CadastrarDisparoCanhao(SequenciaEventos *seq, float instante, int indiceCanhao)
{
    Evento e = {0};
    e.instante = instante;
    e.tipo = EVENTO_DISPARO_CANHAO;
    e.indiceCanhao = indiceCanhao;
    return CadastrarEvento(seq, e);
}

// Ilha: reforcos de marujos durante a travessia (valores EXPERIMENTAIS).
// Os limites ficam em chao firme, porque o marujo ainda nao colide com o cenario.
// A ordem de cadastro nao precisa seguir o tempo: OrdenarEventos arruma.
static void EventosDaIlha(SequenciaEventos *seq)
{
    // Chao firme da ilha: 1400-1650 (antes dos cipos) e 2960-3280 (depois dos cipos).
    CadastrarSurgimentoMarujo(seq, 15.0f, 3200.0f, CHAO_Y, 2965.0f, 3275.0f); // empate em t=15:
    CadastrarSurgimentoMarujo(seq,  5.0f, 1600.0f, CHAO_Y, 1405.0f, 1645.0f);
    CadastrarSurgimentoMarujo(seq, 15.0f, 3000.0f, CHAO_Y, 2965.0f, 3275.0f); // fica nesta ordem
}

void MontarEventosDaFase(SequenciaEventos *seq, int faseId)
{
    InicializarSequencia(seq);
    if (faseId == 2) EventosDaIlha(seq);
    // Navio (fase 3): marujos e disparos de canhao quando a fase existir.

    OrdenarEventos(seq->eventos, seq->quantidade);
    ReiniciarEventos(seq);
}
