# B-Tree em C

Este projeto é uma implementação pura de uma Árvore B (B-Tree) em linguagem C, desenvolvida como exercício de estudo para a disciplina de Estrutura de Dados II.

## Sobre o Projeto

A Árvore B é uma estrutura de dados balanceada amplamente utilizada em sistemas de banco de dados e sistemas de arquivos para gerenciar grandes volumes de dados que não cabem inteiramente na memória principal. Esta implementação visa proporcionar aprendizado prático sobre os conceitos, operações e desafios envolvidos no uso de árvores B.

## Funcionalidades
- **Criação de árvore B** com grau mínimo definido pelo usuário
- **Inserção** de múltiplos valores
- **Busca** de valores
- **Remoção** de valores
- **Impressão** da árvore
- **Liberação de memória**

O menu interativo permite ao usuário realizar operações básicas e visualizar o funcionamento da estrutura.

## Estrutura dos Arquivos

- `b_tree.c` — Implementação das funções principais da árvore B (criação, inserção, busca, remoção, impressão, liberação de memória, etc.)
- `b_tree.h` — Definições das estruturas e protótipos das funções
- `main.c` — Interface de linha de comando para interação com a árvore B

## Como Compilar e Executar

1. **Abra o terminal na pasta `b_tree/`**
2. **Compile os arquivos:**
   ```sh
   gcc main.c b_tree.c -o btree
   ```
3. **Execute o programa:**
   ```sh
   ./btree
   ```
   No Windows, use:
   ```sh
   btree.exe
   ```

## Exemplo de Uso

Ao executar, o programa solicitará o grau mínimo da árvore (T ≥ 2) e apresentará um menu para inserir, buscar, remover e imprimir valores.

## Observações
- O código segue o método de inserção "split antes de inserir" (CLRS), considerado mais eficiente e didático.
- Todos os ponteiros e alocações de memória são tratados para evitar vazamentos.
- Comentários explicativos acompanham as principais funções para facilitar o estudo.