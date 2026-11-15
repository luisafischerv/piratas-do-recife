#include <stdio.h>
#include <string.h>
#include <math.h>
#include "ranking.h"

#define APELIDO_PADRAO "Pirata"

void InicializarRanking(Ranking *ranking)
{
    ranking->quantidade = 0;
    ranking->proximaOrdem = 0;
}

// a deve ficar DEPOIS de b? (tempo maior, ou mesmo tempo e registrado depois)
static bool VemDepois(const Resultado *a, const Resultado *b)
{
    if (a->tempo != b->tempo) return a->tempo > b->tempo;
    return a->ordemRegistro > b->ordemRegistro;
}

void OrdenarRanking(Resultado v[], int n)
{
    for (int i = 1; i < n; i++) {
        Resultado chave = v[i];
        int j = i - 1;

        // Empurra para a direita quem deve ficar depois da chave.
        while (j >= 0 && VemDepois(&v[j], &chave)) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = chave;
    }
}

// Copia ate 12 caracteres e troca os que estragariam o arquivo.
static void LimparApelido(const char *origem, char destino[TAM_APELIDO])
{
    int n = 0;
    if (origem != NULL) {
        while (origem[n] != '\0' && n < TAM_APELIDO - 1) {
            char c = origem[n];
            if (c == ';' || c == '\n' || c == '\r') c = '_';
            destino[n] = c;
            n++;
        }
    }
    destino[n] = '\0';
    if (n == 0) strcpy(destino, APELIDO_PADRAO);
}

bool RegistrarResultado(Ranking *ranking, const char *apelido, double tempo)
{
    if (tempo < 0.0 || isnan(tempo)) return false;

    Resultado novo;
    LimparApelido(apelido, novo.apelido);
    // Arredonda ao milesimo: e o que o arquivo guarda ("%.3f"),
    // entao o valor volta identico ao reabrir o jogo.
    novo.tempo = round(tempo * 1000.0) / 1000.0;
    novo.ordemRegistro = ranking->proximaOrdem++;

    if (ranking->quantidade < MAX_RANKING) {
        ranking->resultados[ranking->quantidade] = novo;
        ranking->quantidade++;
    } else {
        // Cheio: so entra se for estritamente melhor que o 10o.
        // Empate com o 10o nao entra (o mais antigo tem preferencia).
        Resultado *ultimo = &ranking->resultados[MAX_RANKING - 1];
        if (!(novo.tempo < ultimo->tempo)) return false;
        *ultimo = novo;
    }

    OrdenarRanking(ranking->resultados, ranking->quantidade);
    return true;
}

int CarregarRanking(Ranking *ranking, const char *arquivo)
{
    InicializarRanking(ranking);

    FILE *f = fopen(arquivo, "r");
    if (f == NULL) return 0;              // primeira vez jogando: ranking vazio

    char linha[128];
    while (fgets(linha, sizeof linha, f) != NULL) {
        Resultado r;
        if (sscanf(linha, "%12[^;];%lf;%d", r.apelido, &r.tempo, &r.ordemRegistro) != 3)
            continue;                     // linha estragada: ignora
        if (r.tempo < 0.0 || isnan(r.tempo) || r.ordemRegistro < 0)
            continue;

        if (r.ordemRegistro >= ranking->proximaOrdem)
            ranking->proximaOrdem = r.ordemRegistro + 1;

        if (ranking->quantidade < MAX_RANKING) {
            ranking->resultados[ranking->quantidade] = r;
            ranking->quantidade++;
            OrdenarRanking(ranking->resultados, ranking->quantidade);
        } else if (VemDepois(&ranking->resultados[MAX_RANKING - 1], &r)) {
            // Arquivo com mais de 10 linhas: fica so com os 10 melhores.
            ranking->resultados[MAX_RANKING - 1] = r;
            OrdenarRanking(ranking->resultados, ranking->quantidade);
        }
    }
    fclose(f);

    // A cada linha o vetor e reordenado, entao ele termina em ordem
    // mesmo se o arquivo tiver sido editado a mao fora de ordem.
    return ranking->quantidade;
}

bool SalvarRanking(const Ranking *ranking, const char *arquivo)
{
    FILE *f = fopen(arquivo, "w");
    if (f == NULL) return false;

    bool ok = true;
    for (int i = 0; i < ranking->quantidade; i++) {
        const Resultado *r = &ranking->resultados[i];
        if (fprintf(f, "%s;%.3f;%d\n", r->apelido, r->tempo, r->ordemRegistro) < 0)
            ok = false;
    }
    if (fclose(f) != 0) ok = false;
    return ok;
}

void FormatarTempo(double tempo, char *saida, int tamanho)
{
    if (tempo < 0.0) tempo = 0.0;
    long long milesimos = llround(tempo * 1000.0);
    int minutos = (int)(milesimos / 60000);
    int segundos = (int)((milesimos / 1000) % 60);
    int resto = (int)(milesimos % 1000);
    snprintf(saida, (size_t)tamanho, "%02d:%02d.%03d", minutos, segundos, resto);
}
