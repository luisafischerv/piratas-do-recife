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
    assert(fase.largura == 2560.0f && fase.qtdPlataformas > 2);
    assert(fase.pontoInicial.x >= 0 && fase.pontoInicial.x < fase.largura);
    DescarregarFase(&fase);
    CloseWindow();
    puts("Carregamento das fases passou.");
    return 0;
}
