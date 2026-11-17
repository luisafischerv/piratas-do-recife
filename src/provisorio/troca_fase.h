#ifndef TROCA_FASE_PROVISORIA_H
#define TROCA_FASE_PROVISORIA_H

#include "raylib.h"

// Atalho apenas para testes: 0 significa manter a fase atual.
static inline int DestinoTesteFase(int faseAtual)
{
    bool anterior = IsKeyPressed(KEY_LEFT);
    bool proxima = IsKeyPressed(KEY_RIGHT);
    if (anterior == proxima) return 0;
    if (anterior && faseAtual > 1) return faseAtual - 1;
    if (proxima && faseAtual < 3) return faseAtual + 1;
    return 0;
}
#endif
