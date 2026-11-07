# Configuração local deste computador (não versionado — está no .gitignore).
# Baseado em config.mk.example, ajustado para o MSYS2 UCRT64 em C:\msys64.
#
# A Raylib do MSYS2 é encontrada pelo pkg-config, então RAYLIB_DIR fica vazio.
# Se um dia usar a Raylib do instalador oficial (w64devkit), troque por:
#   CC = C:/raylib/w64devkit/bin/gcc.exe
#   RAYLIB_DIR = C:/raylib/raylib/src

CC         = C:/msys64/ucrt64/bin/gcc.exe
PKG_CONFIG = C:/msys64/ucrt64/bin/pkg-config.exe
RAYLIB_DIR =
