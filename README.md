# README – Como Rodar o Programa

Este documento contém instruções para compilar e executar o programa `esqueleto.c`, que indexa palavras de um arquivo texto e permite buscas interativas usando duas estruturas de dados: **árvore binária de busca** e **lista encadeada**.

---

## 1. Pré-requisitos

- **Compilador GCC**  
  Certifique-se de ter o GCC instalado (Linux, WSL ou MinGW no Windows).

- **Arquivo de texto**  
  Crie ou utilize um arquivo `.txt` com o conteúdo a ser indexado.  
  Um exemplo (`texto.txt`) já está incluído no projeto e pode ser editado livremente.

---

## 2. Como Compilar

No terminal, acesse o diretório onde está o arquivo `esqueleto.c` e execute:

```bash
gcc esqueleto.c -o esqueleto -lm
```

**Explicação:**
- `esqueleto.c`: código-fonte principal do programa
- `-o esqueleto`: gera um executável com o nome `esqueleto`
- `-lm`: inclui a biblioteca matemática (necessária para `clock_gettime`)

---

## 3. Como Executar

Rode o programa com:

```bash
./esqueleto <nome_do_arquivo> <tipo_de_indice>
```

- `<nome_do_arquivo>`: nome do arquivo `.txt` a ser indexado (ex: `texto.txt`)
- `<tipo_de_indice>`: `arvore` ou `lista`

**Exemplos:**
```bash
./esqueleto texto.txt arvore
./esqueleto texto.txt lista
```

---

## 4. Interagindo com o Programa

Após a carga do arquivo e construção do índice, o programa entra no modo interativo:

```
>
```

Você pode utilizar dois comandos:

### 🔍 `busca <palavra>`

Retorna o número de ocorrências da palavra e as linhas em que ela aparece.

**Exemplo:**
```
> busca algoritmo
Existem 3 ocorrências da palavra 'algoritmo' na(s) seguinte(s) linha(s):
00001: Esta é uma linha com a palavra algoritmo.
00003: Outra linha com algoritmo novamente.
00007: Mais uma linha com algoritmo.
Tempo de busca: 1 ms
```

### ❌ `fim`

Encerra o programa.

---

## 5. Regras e Observações

- Palavras são tratadas de forma **case-insensitive**
- **Pontuação é ignorada** (ex: ".", ",", ":", etc.)
- Palavras compostas com hífen são **separadas**
- A busca considera **apenas palavras exatas**  
  Exemplo: `algorithm` ≠ `algorithms`
- Comandos inválidos devem ser rejeitados com mensagem de erro
- O programa exibe:
  - Número de linhas do arquivo
  - Total de palavras indexadas
  - Tempo de carregamento (em ms)
  - Altura da árvore (somente se `arvore` for usado)
  - Tempo de cada busca (em ms)

---

## 6. Autoria

EP desenvolvido para a disciplina **ACH2023 – Algoritmos e Estruturas de Dados I**, 2º semestre de 2024.

Grupo:  
- Diego Aguiar
- Gustavo Jun
