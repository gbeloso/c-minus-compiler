# C-Minus Compiler

Compilador para a linguagem **C-** (C-Minus) que gera código de máquina para um
**processador customizado** (projeto desenvolvido na disciplina de Laboratório de
Sistemas Operacionais da Unifesp). O compilador percorre todas as fases clássicas —
análise léxica, sintática, semântica, geração de código intermediário, geração de
assembly e, por fim, geração do binário de 32 bits que é carregado e executado
diretamente pelo processador.

## Visão geral do pipeline

O fluxo de compilação acontece em sequência dentro do `main`:

```
código-fonte C-   →  análise léxica (Flex)
                  →  análise sintática (Bison)  →  AST
                  →  análise semântica          →  tabela de símbolos
                  →  código intermediário       →  quádruplas
                  →  geração de assembly        →  alocação de registradores/memória
                  →  geração de binário         →  32 bits por instrução
```

Cada etapa produz um arquivo de saída na pasta `saida/`, o que facilita depurar o
resultado intermediário de cada fase.

## Estrutura do projeto

```
src/
  scan.l          # Especificação Flex (analisador léxico)
  parser.y        # Gramática Bison (analisador sintático)
  globals.h       # Tipos e estruturas globais (AST, tabela, quádruplas, etc.)
  util.c/.h       # Funções utilitárias e impressão da AST
  symtab.c/.h     # Tabela de símbolos (hash)
  analyze.c/.h    # Análise semântica
  inter_code.c/.h # Geração de código intermediário (quádruplas)
  assembly.c/.h   # Geração de código assembly
  mem.c/.h        # Alocação/mapeamento de memória
  binary.c/.h     # Geração do código binário de 32 bits
  main.c          # Orquestra todo o pipeline

exemplos/         # Programas C- de teste (entrada do compilador)
saida/            # Arquivos gerados por cada fase (ignorado pelo git)
obj/              # Binário compilado (ignorado pelo git)
Makefile          # Build e execução
```

## Arquivos de saída (`saida/`)

| Arquivo           | Conteúdo                                                    |
|-------------------|-------------------------------------------------------------|
| `tokens.txt`      | Tokens reconhecidos pela análise léxica                     |
| `arvore.txt`      | Árvore sintática abstrata (AST)                             |
| `tabela.txt`      | Tabela de símbolos                                           |
| `inter_code.txt`  | Código intermediário (quádruplas)                           |
| `assembly.txt`    | Código assembly do processador alvo                         |
| `mem.txt`         | Mapeamento de memória                                       |
| `bin.txt`         | Código binário final (uma instrução de 32 bits por linha)   |

## Linguagem suportada (C-)

Subconjunto de C contendo:

- Tipos `int` e `void`
- Declaração de variáveis escalares e **vetores** (`int x[3];`)
- Funções com parâmetros e `return`
- Estruturas de controle `if` / `else` e `while`
- Operadores aritméticos (`+ - * /`) e relacionais (`< <= > >= == !=`)
- Funções de E/S embutidas: `input()` (leitura) e `output()` (escrita)
- Comentários no formato `/* ... */`

## Processador alvo

O compilador gera instruções de **32 bits** para o processador customizado. Os 8 bits
mais significativos formam o opcode. Registradores são codificados em 6 bits
(`convertReg`), com `fp` = `111101` (61) e `sp` = `111110` (62).

### Conjunto de instruções (opcodes)

| Opcode     | Instrução | Descrição                                   |
|------------|-----------|---------------------------------------------|
| `00000000` | `add`     | Soma entre registradores                    |
| `00000001` | `addi`    | Soma com imediato                           |
| `00000010` | `sub`     | Subtração entre registradores               |
| `00000011` | `subi`    | Subtração com imediato                       |
| `00000100` | `cmp`     | Comparação entre registradores              |
| `00000101` | `cmpi`    | Comparação com imediato                      |
| `00000110` | `mov`     | Cópia entre registradores                   |
| `00000111` | `mult`    | Multiplicação entre registradores           |
| `00001000` | `multi`   | Multiplicação com imediato                   |
| `00001001` | `div`     | Divisão entre registradores                 |
| `00001010` | `divi`    | Divisão com imediato                         |
| `00001011` | `str`     | Store (escrita em memória)                   |
| `00001100` | `ldr`     | Load (leitura de memória)                    |
| `00001101` | `ldrb`    | Load de imediato em registrador              |
| `00001110` | `beq`     | Desvio se igual                              |
| `00001111` | `bnq`     | Desvio se diferente                          |
| `00010000` | `bgt`     | Desvio se maior                              |
| `00010001` | `blt`     | Desvio se menor                              |
| `00010010` | `jmp`     | Desvio incondicional                         |
| `00010011` | `jal`     | Desvio com salvamento de retorno (call)      |
| `00010100` | `jmpr`    | Desvio para endereço em registrador (return) |
| `00010101` | `in`      | Leitura de entrada (`input`)                 |
| `00010110` | `out`     | Escrita de saída (`output`)                  |
| `00010111` | `posin`   | Pós-entrada                                  |
| `00011001` | `halt`    | Encerra a execução                           |
| `00011000` | `nop`     | Nenhuma operação (opcode padrão)             |

## Dependências

- `gcc`
- `flex`
- `bison`
- `make`

Em distribuições baseadas em Debian/Ubuntu:

```bash
sudo apt install build-essential flex bison
```

## Como compilar e executar

O ponto de entrada compilado espera a existência das pastas `saida/` e `obj/`. Se
elas não existirem, crie-as antes de rodar.

### 1. Compilar o compilador

```bash
make
```

Esse alvo roda o Flex, o Bison e o `gcc`, gerando o executável em `obj/main.o`.

### 2. Escolher o programa de entrada

Por padrão, o `main` lê o arquivo `exemplos/exemplo.c`. Para compilar outro
programa, edite o caminho em `src/main.c` (variável `fonte`) e recompile, ou
substitua o conteúdo de `exemplos/exemplo.c`.

### 3. Executar a compilação

```bash
make run
```

Ao final, os arquivos em `saida/` conterão o resultado de cada fase. O binário
final — pronto para ser carregado no processador — fica em `saida/bin.txt`, com
uma instrução de 32 bits por linha.

### Depuração

```bash
make debug
```

Abre o executável no `gdb` para depuração passo a passo.

## Observações

- As pastas `obj/` e `saida/` estão listadas no `.gitignore`, pois contêm
  artefatos gerados durante o build e a compilação.
- Os comandos do `Makefile` assumem um ambiente Unix-like (Linux/WSL). No Windows,
  recomenda-se usar o WSL ou um ambiente com `flex`, `bison` e `gcc` disponíveis.
```