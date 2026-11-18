# Piratas do Recife: A Maldição da Maré Negra

Jogo de plataforma 2D para PC desenvolvido em C com Raylib.

## Requisitos

- Compilador C compatível com a biblioteca Raylib instalada.
- GNU Make disponível no terminal.
- Raylib disponível pelo `pkg-config` ou instalada em `C:/raylib/raylib/src` no Windows.

## Como executar

Abra um terminal na pasta raiz do projeto e execute:

```bash
make run
```

O comando compila e inicia o jogo. Para apenas compilar, use `make`. Para remover os arquivos compilados, use `make clean`.

No Windows, a instalação padrão do Raylib em `C:/raylib/raylib/src` e o compilador em `C:/raylib/w64devkit/bin` são detectados automaticamente. Se a biblioteca estiver em outra pasta, informe o caminho:

```bash
make run RAYLIB_DIR=C:/caminho/para/raylib/src
```

Se o comando `make` não for reconhecido, instale o GNU Make ou adicione a pasta que contém o executável ao PATH e abra um novo terminal.

Execute os comandos na pasta raiz para que o jogo encontre a pasta `assets`.

## Controles

- **A / D** — mover.
- **Espaço** — pular ou soltar o cipó.
- **Mouse esquerdo** — atacar com a espada.
- **Mouse direito** — atirar com a pistola a partir da fase 2.
- **E** — agarrar cipós e cordas.
- **W / S** — subir e descer nas cordas.
- **Esc** — pausar.
- **Enter** — avançar nas telas de história e instruções.
