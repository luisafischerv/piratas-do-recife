#include "tiros.h"
#include "inimigos_ia.h"
#include <math.h>

#define INTERVALO_PISTOLA 0.35f
#define VELOCIDADE_TIRO 550.0f
#define AVISO_ATIRADOR 0.8f
#define RECARGA_ATIRADOR 1.5f
#define VELOCIDADE_ATIRADOR 60.0f

static bool Disparar(SistemaTiros *s, Rectangle corpo, int direcao, bool jogador)
{
    for (int i = 0; i < MAX_TIROS; i++) {
        if (s->tiros[i].ativo) continue;
        float x = direcao > 0 ? corpo.x + corpo.width : corpo.x - 12.0f;
        s->tiros[i] = (Tiro){ {x, corpo.y + corpo.height * ALTURA_TIRO_RELATIVA, 12.0f, 8.0f},
                              direcao * VELOCIDADE_TIRO, true, jogador };
        return true;
    }
    return false;
}

void InicializarTiros(SistemaTiros *s) { *s = (SistemaTiros){0}; }

bool AtualizarPistola(SistemaTiros *s, const Jogador *j, bool pressionado, float dt)
{
    if (s->recarga > 0.0f) s->recarga -= dt;
    if (pressionado && s->recarga <= 0.0f && Disparar(s, j->corpo, j->direcao, true)) {
        s->recarga = INTERVALO_PISTOLA;
        return true;
    }
    return false;
}

Inimigo CriarAtirador(float x, float chaoY)
{
    Inimigo i = {0};
    i.tipo = INIMIGO_ATIRADOR;
    i.corpo = (Rectangle){x, chaoY - 64.0f, 44.0f, 64.0f};
    i.direcao = -1;
    i.saude = 3;
    i.estado = ESTADO_RECUPERACAO;
    i.timerEstado = 0.8f;
    i.limiteEsquerdo = x - 100.0f;
    i.limiteDireito = x + 80.0f;
    return i;
}

void AtualizarAtiradores(SistemaTiros *s, ListaInimigos *lista, const Jogador *j, float dt)
{
    for (NoInimigo *no = lista->inicio; no; no = no->proximo) {
        Inimigo *i = &no->dados;
        if (i->tipo != INIMIGO_ATIRADOR || i->saude <= 0 || i->aguardaProximidade) continue;
        i->caminhando = false;
        i->timerEstado -= dt;
        if (i->estado == ESTADO_PREPARACAO) {
            // A direcao fica fixa durante o aviso para permitir a esquiva.
            if (i->timerEstado <= 0.0f) {
                Disparar(s, i->corpo, i->direcao, false);
                i->golpeJaAcertou = true; // indica que a recuperacao segue um disparo
                i->estado = ESTADO_RECUPERACAO;
                i->timerEstado = RECARGA_ATIRADOR;
            }
        } else if (i->timerEstado <= 0.0f &&
                   fabsf(CentroXRetangulo(j->corpo) - CentroXRetangulo(i->corpo)) < 650.0f &&
                   fabsf(CentroYRetangulo(j->corpo) - CentroYRetangulo(i->corpo)) < 100.0f) {
            i->direcao = CentroXRetangulo(j->corpo) >= CentroXRetangulo(i->corpo) ? 1 : -1;
            i->estado = ESTADO_PREPARACAO;
            i->golpeJaAcertou = false;
            i->timerEstado = AVISO_ATIRADOR;
        } else if (i->estado == ESTADO_RECUPERACAO &&
                   (!i->golpeJaAcertou || i->timerEstado <= 1.15f)) {
            // Patrulha curta entre tiros; para durante o aviso e o recuo.
            float xAnterior = i->corpo.x;
            MoverInimigoNosLimites(i, i->direcao * VELOCIDADE_ATIRADOR * dt);
            i->caminhando = i->corpo.x != xAnterior;
            if (i->caminhando) i->tempoCaminhada += dt;
            if (i->corpo.x <= i->limiteEsquerdo) i->direcao = 1;
            if (i->corpo.x + i->corpo.width >= i->limiteDireito) i->direcao = -1;
        }
    }
}

// Distancia percorrida ate o primeiro contato horizontal, evitando atravessar alvos finos.
static float DistanciaContato(Rectangle tiro, Rectangle alvo, float deslocamento)
{
    if (tiro.y + tiro.height <= alvo.y || tiro.y >= alvo.y + alvo.height) return -1.0f;
    if (RetangulosSobrepostos(tiro, alvo)) return 0.0f;
    float distancia = deslocamento > 0.0f ? alvo.x - (tiro.x + tiro.width) : tiro.x - (alvo.x + alvo.width);
    return distancia >= 0.0f && distancia <= fabsf(deslocamento) ? distancia : -1.0f;
}

// O corpo usado na movimentacao e menor que a arte. Para tiros, inclui a cabeca
// e o tronco desenhados acima dele, sem tornar armas e acessorios vulneraveis.
static Rectangle AreaDanoPorTiro(Rectangle corpo, float extensaoSuperior)
{
    corpo.y -= extensaoSuperior;
    corpo.height += extensaoSuperior;
    return corpo;
}

int AtualizarTiros(SistemaTiros *s, const Fase *fase, ListaInimigos *lista, const Jogador *j, float dt)
{
    int dano = 0;
    for (int k = 0; k < MAX_TIROS; k++) {
        Tiro *t = &s->tiros[k];
        if (!t->ativo) continue;
        float deslocamento = t->velocidade * dt;
        float menor = fabsf(deslocamento) + 1.0f;
        Inimigo *atingido = NULL;
        bool atingiuJogador = false;
        for (int p = 0; p < fase->qtdPlataformas; p++) {
            if (!fase->plataformas[p].ativa) continue;
            float d = DistanciaContato(t->area, fase->plataformas[p].area, deslocamento);
            if (d >= 0.0f && d < menor) menor = d;
        }
        if (t->doJogador) {
            for (NoInimigo *no = lista->inicio; no; no = no->proximo) {
                if (no->dados.saude <= 0) continue;
                Rectangle alvo = AreaDanoPorTiro(no->dados.corpo, 30.0f);
                float d = DistanciaContato(t->area, alvo, deslocamento);
                if (d >= 0.0f && d < menor) { menor = d; atingido = &no->dados; }
            }
        } else {
            Rectangle alvo = AreaDanoPorTiro(j->corpo, 30.0f);
            float d = DistanciaContato(t->area, alvo, deslocamento);
            if (d >= 0.0f && d < menor) { menor = d; atingiuJogador = true; }
        }
        if (menor <= fabsf(deslocamento)) {
            t->ativo = false;
            if (atingido) { atingido->saude--; atingido->tempoDano = 0.15f; }
            if (atingiuJogador) dano++;
        } else {
            t->area.x += deslocamento;
            if (t->area.x + t->area.width < 0 || t->area.x > fase->largura) t->ativo = false;
        }
    }
    return dano;
}

