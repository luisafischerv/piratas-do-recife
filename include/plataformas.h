#ifndef PLATAFORMAS_H
#define PLATAFORMAS_H

#include <stdbool.h>
#include "raylib.h"

typedef enum TipoPlataforma {
    PLAT_CHAO,
    PLAT_FIXA,
    PLAT_CAIXA,
    PLAT_FRAGIL
} TipoPlataforma;

typedef enum EstadoPlataforma {
    PLAT_NORMAL,
    PLAT_AVISO,
    PLAT_CAIDA
} EstadoPlataforma;

typedef struct Plataforma {
    Rectangle area;
    TipoPlataforma tipo;
    EstadoPlataforma estado;
    float tempoAviso;
    bool ativa;
} Plataforma;

#endif