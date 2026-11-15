#include <stdio.h>
#include <string.h>
#include "ranking.h"

static int falhas = 0;

#define CONFERIR(cond) do { \
    if (!(cond)) { printf("  FALHOU (linha %d): %s\n", __LINE__, #cond); falhas++; } \
} while (0)

#define ARQ_TESTE "teste_ranking_tmp.txt"

static void Mostrar(const Ranking *r)
{
    char t[16];
    for (int i = 0; i < r->quantidade; i++) {
        FormatarTempo(r->resultados[i].tempo, t, sizeof t);
        printf("  %2d. %-12s %s (reg %d)\n", i + 1, r->resultados[i].apelido, t,
               r->resultados[i].ordemRegistro);
    }
}

int main(void)
{
    Ranking r, lido;
    char t[16];

    printf("1) Arquivo que nao existe: ranking vazio, sem quebrar\n");
    remove(ARQ_TESTE);
    CONFERIR(CarregarRanking(&r, ARQ_TESTE) == 0);
    CONFERIR(r.quantidade == 0 && r.proximaOrdem == 0);

    printf("2) Insercoes fora de ordem e empate (mais antigo antes)\n");
    InicializarRanking(&r);
    RegistrarResultado(&r, "Ana", 300.0);
    RegistrarResultado(&r, "Bia", 120.5);
    RegistrarResultado(&r, "Caio", 300.0);   // empata com Ana, mas e mais novo
    RegistrarResultado(&r, "Duda", 90.0);
    Mostrar(&r);
    CONFERIR(strcmp(r.resultados[0].apelido, "Duda") == 0);
    CONFERIR(strcmp(r.resultados[1].apelido, "Bia") == 0);
    CONFERIR(strcmp(r.resultados[2].apelido, "Ana") == 0);
    CONFERIR(strcmp(r.resultados[3].apelido, "Caio") == 0);

    printf("3) Apelido: corta em 12, troca ';' e vazio vira Pirata\n");
    InicializarRanking(&r);
    RegistrarResultado(&r, "NomeMuitoGrandeDemais", 10.0);
    RegistrarResultado(&r, "a;b", 20.0);
    RegistrarResultado(&r, "", 30.0);
    CONFERIR(strcmp(r.resultados[0].apelido, "NomeMuitoGra") == 0);
    CONFERIR(strlen(r.resultados[0].apelido) == 12);
    CONFERIR(strcmp(r.resultados[1].apelido, "a_b") == 0);
    CONFERIR(strcmp(r.resultados[2].apelido, "Pirata") == 0);

    printf("4) So os 10 melhores ficam\n");
    InicializarRanking(&r);
    for (int i = 1; i <= 10; i++) RegistrarResultado(&r, "J", i * 10.0); // 10..100
    CONFERIR(r.quantidade == 10);
    CONFERIR(!RegistrarResultado(&r, "Pior", 500.0));       // pior que o 10o
    CONFERIR(!RegistrarResultado(&r, "Empate", 100.0));     // empate com o 10o nao entra
    CONFERIR(RegistrarResultado(&r, "Melhor", 5.0));        // entra e expulsa o 100
    CONFERIR(r.quantidade == 10);
    CONFERIR(strcmp(r.resultados[0].apelido, "Melhor") == 0);
    CONFERIR(r.resultados[9].tempo == 90.0);

    printf("5) Salvar, recarregar e comparar campo a campo\n");
    InicializarRanking(&r);
    RegistrarResultado(&r, "Luisa", 583.2504);   // arredonda para 583.250
    RegistrarResultado(&r, "Nico", 583.25);      // empata depois de arredondar
    RegistrarResultado(&r, "Joao", 200.0);
    CONFERIR(SalvarRanking(&r, ARQ_TESTE));
    CONFERIR(CarregarRanking(&lido, ARQ_TESTE) == 3);
    Mostrar(&lido);
    for (int i = 0; i < 3; i++) {
        CONFERIR(strcmp(lido.resultados[i].apelido, r.resultados[i].apelido) == 0);
        CONFERIR(lido.resultados[i].tempo == r.resultados[i].tempo);
        CONFERIR(lido.resultados[i].ordemRegistro == r.resultados[i].ordemRegistro);
    }
    CONFERIR(lido.proximaOrdem == 3);

    printf("6) Depois de reabrir, empate novo vai para depois dos antigos\n");
    RegistrarResultado(&lido, "Novo", 200.0);
    CONFERIR(strcmp(lido.resultados[0].apelido, "Joao") == 0);
    CONFERIR(strcmp(lido.resultados[1].apelido, "Novo") == 0);
    CONFERIR(lido.resultados[1].ordemRegistro == 3);

    printf("7) Linhas estragadas sao ignoradas\n");
    FILE *f = fopen(ARQ_TESTE, "w");
    fprintf(f, "Bom;50.000;0\n");
    fprintf(f, "linha sem separador\n");
    fprintf(f, "Negativo;-3.0;1\n");
    fprintf(f, ";10.0;2\n");
    fprintf(f, "OrdemRuim;20.0;-1\n");
    fprintf(f, "Outro;40.500;7\n");
    fclose(f);
    CONFERIR(CarregarRanking(&lido, ARQ_TESTE) == 2);
    CONFERIR(strcmp(lido.resultados[0].apelido, "Outro") == 0);
    CONFERIR(lido.proximaOrdem == 8);

    printf("8) Arquivo com mais de 10 linhas: fica com os 10 melhores\n");
    f = fopen(ARQ_TESTE, "w");
    for (int i = 0; i < 12; i++) fprintf(f, "J%d;%d.000;%d\n", i, 120 - i * 10, i); // 120..10
    fclose(f);
    CONFERIR(CarregarRanking(&lido, ARQ_TESTE) == 10);
    CONFERIR(lido.resultados[0].tempo == 10.0);
    CONFERIR(lido.resultados[9].tempo == 100.0);
    CONFERIR(lido.proximaOrdem == 12);

    printf("9) Falha ao salvar em pasta inexistente retorna false\n");
    CONFERIR(!SalvarRanking(&r, "pasta_que_nao_existe/ranking.txt"));

    printf("10) FormatarTempo\n");
    FormatarTempo(583.25, t, sizeof t);
    printf("  583.25 -> %s\n", t);
    CONFERIR(strcmp(t, "09:43.250") == 0);
    FormatarTempo(0.0, t, sizeof t);
    CONFERIR(strcmp(t, "00:00.000") == 0);

    remove(ARQ_TESTE);
    if (falhas == 0) printf("\nOK: todos os testes do ranking passaram.\n");
    else printf("\n%d verificacao(oes) falharam.\n", falhas);
    return falhas;
}
