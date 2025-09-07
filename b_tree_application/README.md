# Sistema de Cadastro de Alunos com Árvore B

Este projeto é uma aplicação prática utilizando uma Árvore B para indexação e busca eficiente de alunos por RA (Registro Acadêmico), desenvolvida como exercício para a disciplina de Estrutura de Dados II.

## Sobre o Projeto

Após a implementação de uma Árvore B pura, o objetivo deste sistema é demonstrar seu uso real em um cadastro de alunos, onde cada aluno possui RA, nome e email. Os dados dos alunos são armazenados em arquivo binário e os índices (RA e offset no arquivo) são gerenciados por uma Árvore B.

## Funcionalidades
- **Cadastro de alunos** (RA, nome, email)
- **Busca de alunos por RA**
- **Impressão de todos os alunos cadastrados**
- **Remoção de alunos**
- **Persistência dos dados em arquivo binário**
- **Menu interativo para operações**

## Estrutura dos Arquivos
- `main.c` — Interface de linha de comando e menu de operações
- `alunos.c` / `alunos.h` — Funções para manipulação do arquivo binário de alunos
- `b_tree.c` / `b_tree.h` — Implementação da Árvore B adaptada para armazenar offsets dos registros

## Como Compilar e Executar

1. **Abra o terminal na pasta `b_tree_application/`**
2. **Compile os arquivos:**
   ```sh
   gcc main.c b_tree.c alunos.c -o alunos_btree
   ```
3. **Execute o programa:**
   ```sh
   ./alunos_btree
   ```
   No Windows, use:
   ```sh
   alunos_btree.exe
   ```

## Exemplo de Uso

- O programa solicitará o grau mínimo da árvore (T ≥ 2).
- Permite inserir alunos, buscar por RA, listar todos e remover.
- Os dados são persistidos no arquivo `alunos.dat`.

## Observações
- O índice B-Tree armazena o RA e o offset do registro no arquivo binário, permitindo buscas rápidas sem carregar todos os dados na memória.
- O código é comentado e modularizado para facilitar o estudo e a manutenção.
- Ideal para aprendizado de estruturas de dados aplicadas a problemas reais.