#include "eventos.h"

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

