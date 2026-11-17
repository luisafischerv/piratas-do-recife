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

// Navio: tripulacao surgindo e canhoes anunciados (valores EXPERIMENTAIS).
// POSICOES PROVISORIAS: supoe convés continuo de 0 a 3200 com chao em y = 620;
// ajustar quando a Pessoa 2 criar a fase 3. Indices dos canhoes: 0 (x=1150),
// 1 (x=2050) e 2 (x=2950), definidos em MontarCanhoesDaFase (canhao.c).
static void EventosDoNavio(SequenciaEventos *seq)
{
    // Canhoes: cada um dispara em ciclos; o aviso de 1 s vem antes de cada tiro.
    CadastrarDisparoCanhao(seq,  7.0f, 0);
    CadastrarDisparoCanhao(seq,  3.0f, 0);
    CadastrarDisparoCanhao(seq, 11.0f, 0);
    CadastrarDisparoCanhao(seq, 13.0f, 1);
    CadastrarDisparoCanhao(seq,  9.0f, 1);
    CadastrarDisparoCanhao(seq, 17.0f, 1);
    CadastrarDisparoCanhao(seq, 19.0f, 2);
    CadastrarDisparoCanhao(seq, 15.0f, 2);
    CadastrarDisparoCanhao(seq, 23.0f, 2);

    // Tripulacao: reforcos ao longo do convés.
    CadastrarSurgimentoMarujo(seq, 18.0f, 3000.0f, CHAO_Y, 2800.0f, 3150.0f);
    CadastrarSurgimentoMarujo(seq,  6.0f, 1500.0f, CHAO_Y, 1300.0f, 1900.0f);
    CadastrarSurgimentoMarujo(seq, 12.0f, 2400.0f, CHAO_Y, 2200.0f, 2800.0f); // empate em t=12:
    CadastrarSurgimentoMarujo(seq, 12.0f, 2600.0f, CHAO_Y, 2200.0f, 2800.0f); // fica nesta ordem
}

void MontarEventosDaFase(SequenciaEventos *seq, int faseId)
{
    (void)faseId;
    InicializarSequencia(seq);
    // A fase 3 atual usa a sequencia vertical de canhoes_escalada.c.

    OrdenarEventos(seq->eventos, seq->quantidade);
    ReiniciarEventos(seq);
}

// Preparacao do conves horizontal, ainda nao conectada a escalada atual.
void MontarEventosDoConves(SequenciaEventos *seq)
{
    InicializarSequencia(seq);
    EventosDoNavio(seq);
    OrdenarEventos(seq->eventos, seq->quantidade);
    ReiniciarEventos(seq);
}
