#include <assert.h>
#include <stdio.h>
#include "fase.h"

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_HIDDEN);
    InitWindow(1280, 720, "Teste de fases");
    Fase fase = {0};
    assert(CarregarFase(&fase, 1));
    assert(fase.id == 1 && fase.largura == 2560.0f && fase.areaSaida.width > 0);
    DescarregarFase(&fase);
    assert(CarregarFase(&fase, 2));
    assert(fase.id == 2 && fase.fundo.id && fase.fundo2.id);
    assert(fase.largura == 3280.0f && fase.qtdPlataformas > 2);
    assert(fase.pontoInicial.x >= 0 && fase.pontoInicial.x < fase.largura);
    assert(fase.areaSaida.width > 0);
    DescarregarFase(&fase);
    assert(CarregarFase(&fase, 3));
    assert(fase.fundo.id && fase.qtdPlataformas == 1);
    assert(fase.pontoInicial.x >= fase.plataformas[0].area.x &&
           fase.pontoInicial.x < fase.plataformas[0].area.x + fase.plataformas[0].area.width);
    assert(fase.pontoInicial.y < fase.plataformas[0].area.y);
    DescarregarFase(&fase);
    CloseWindow();
    puts("Carregamento das fases passou.");
    return 0;
}
