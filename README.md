# Piratas do Recife: A Maldição da Maré Negra

Jogo de plataforma 2D em **C** com **Raylib**, desenvolvido para a disciplina de **Algoritmos e Estrutura de Dados**.

O Capitão Avelino retirou um sino antigo das ruínas do mangue e despertou a Maré Negra. Nico Maré, jovem navegador recifense, precisa atravessar o cais, a ilha e o navio para derrotar o capitão e libertar o Recife da maldição, no menor tempo possível.

**Entrega:** 18/11/2026

---

## Equipe

| Integrante | Identificador | Responsabilidade |
| --- | --- | --- |
| _Nome completo_ | (_id_) | Pessoa 1: jogador, pistola e Capitão Avelino |
| _Nome completo_ | (_id_) | Pessoa 2: cenário, colisões, ilha e arena |
| _Nome completo_ | (_id_) | Pessoa 3: inimigos, espada, eventos e ranking |
| _Nome completo_ | (_id_) | Pessoa 4: partida, interface, progressão e distribuição |

---

## Sobre o jogo

- Plataforma lateral 2D, single player, com combate e travessia.
- Três fases: **Cais do Recife**, **Ilha das Marés Perdidas** e **Navio da Maré Negra**, terminando na batalha contra o **Capitão Avelino** (três estágios).
- Três pontos de saúde por tentativa, tentativas ilimitadas e reinício rápido.
- Cronômetro que acumula tentativas e congela em pausas e transições.
- Ranking local com os 10 menores tempos, salvo entre execuções.

### Controles

| Tecla | Ação |
| --- | --- |
| `A` / `D` | Mover para a esquerda / direita |
| `Espaço` | Pular |
| Clique esquerdo | Golpe de espada |
| Botão direito (segurar) | Tiros de pistola (a partir da ilha) |
| `Esc` | Pausar |

---

## Estruturas de dados e algoritmos

| Requisito | Onde é usado |
| --- | --- |
| **Lista duplamente encadeada** | Gerencia os inimigos ativos (inserção, remoção e atualização sem deslocar elementos) |
| **Insertion Sort (1)** | Ordena os eventos de cada fase (surgimento de inimigos e disparos de canhão) pelo instante de ativação |
| **Insertion Sort (2)** | Ordena o ranking pelo tempo crescente, mantendo o registro mais antigo primeiro em caso de empate |
| **Structs** | Jogador, inimigos, plataformas, projéteis, eventos, chefe, resultados e estado da partida |
| **Vetores** | Plataformas, projéteis, eventos e resultados (conjuntos de tamanho limitado) |

O comportamento dos inimigos e do chefe usa máquinas de estados em C (patrulhar, preparar, atacar, recuperar). Nenhuma IA generativa ou API externa é usada durante a partida.

---

## Estrutura do repositório

```
piratas-do-recife/
├── include/          # Cabeçalhos (.h)
│   ├── tipos.h       # Structs compartilhadas — mudanças combinadas com o grupo
│   ├── jogador.h
│   ├── projeteis.h
│   ├── chefe.h
│   ├── fase.h
│   ├── colisao.h
│   ├── plataformas.h
│   ├── inimigos.h
│   ├── combate.h
│   ├── eventos.h
│   ├── ranking.h
│   ├── partida.h
│   ├── interface.h
│   └── animacao.h
├── src/              # Implementações (.c) com os mesmos nomes + main.c
├── assets/           # Sprites, cenários, fontes e sons
├── tests/            # Testes isolados (ex.: teste_lista.c, teste_ordenacao.c)
└── README.md
```

O arquivo do ranking é criado automaticamente na primeira vitória e não deve ser versionado.

---

## Como compilar

Pré-requisitos: compilador C (GCC ou Clang) e **Raylib** instalada. Versão da Raylib usada pelo grupo: _a definir_.

**Linux**

```bash
gcc src/*.c -Iinclude -o piratas -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./piratas
```

**Windows (MinGW / w64devkit)**

```bash
gcc src/*.c -Iinclude -o piratas.exe -lraylib -lopengl32 -lgdi32 -lwinmm
piratas.exe
```

**macOS (Raylib pelo Homebrew)**

```bash
gcc src/*.c -Iinclude -I/opt/homebrew/include -L/opt/homebrew/lib -o piratas \
    -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
./piratas
```

Execute o jogo a partir da raiz do repositório para que a pasta `assets/` seja encontrada.

### Testes isolados

```bash
gcc tests/teste_lista.c src/inimigos.c -Iinclude -o teste_lista -fsanitize=address -g
./teste_lista
```

Os testes de estrutura de dados não dependem da janela da Raylib.

---

## Fluxo de trabalho

- A branch `main` **sempre compila e roda**.
- Cada tarefa tem sua própria branch: `p<numero>/<tarefa>` (ex.: `p3/lista-inimigos`, `p1/pistola`).
- Integração por pull request, com revisão cruzada: **1 revisa 2, 2 revisa 3, 3 revisa 4, 4 revisa 1**.
- Mudanças em `tipos.h` ou em assinaturas usadas por outra pessoa são combinadas com o grupo antes do merge.
- Quadro de tarefas: A fazer → Em andamento → Em revisão → Concluído.

### Convenções de código

- Funções em PascalCase em português (`AtualizarJogador`, `InserirInimigo`); variáveis em camelCase.
- Separar atualização (`Atualizar...`) de desenho (`Desenhar...`) e usar o tempo do quadro (`dt`) no movimento.
- Todo `malloc` tem seu `free`; ao remover inimigos durante a iteração, guardar o `proximo` antes.
- Dano ao jogador passa sempre por `AplicarDanoJogador`.
- Valores de equilíbrio (velocidade, dano, tempos de aviso) são experimentais até serem calibrados jogando.

---

## Cronograma

| Sprint | Período | Entrega |
| --- | --- | --- |
| 0 | 29/10 a 31/10 | `tipos.h` fechado e janela abrindo nos 4 computadores |
| 1 | 01/11 a 06/11 | Base jogável do cais (movimento, colisões, marujo, espada, saúde, pausa, reinício) |
| 2 | 07/11 a 11/11 | Três fases conectadas (pistola, ilha, eventos ordenados, canhões, navio) |
| 3 | 12/11 a 15/11 | Capitão Avelino, convocações, ranking e final |
| 4 | 16/11 a 18/11 | Testes, ajustes, executável testado em outro computador e entrega |

---

## Referências

- [Raylib — cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html)
- [Pat Morin — Open Data Structures](https://opendatastructures.org/)
- Inspirações visuais e de jogabilidade: *Cuphead* (Studio MDHR, 2017), *Hollow Knight* (Team Cherry, 2017) e *Piratas do Caribe: A Maldição do Pérola Negra* (2003)
- Ambientação inspirada no Recife e na Rua da Aurora

Personagens, nomes e artes são próprios do projeto. A origem e a licença de imagens, fontes, músicas e efeitos de terceiros são registradas em `assets/CREDITOS.md`.
