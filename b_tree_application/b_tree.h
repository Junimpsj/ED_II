#ifndef B_TREE_H
#define B_TREE_H

typedef struct NoArvB {
    int n;
    int folha;
    int *chave; //isso aqui é no máximo 2t-1
    long *offset; //offset do registro no arquivo
    struct NoArvB **filho; //isso aqui é 2t, tem um apontador a mais pros filhos
} NoArvB;

typedef struct {
    int t;
    NoArvB *raiz;
} ArvoreB;

//cabeçalho das funções de criar um novo nó e criar a árvore
ArvoreB *criar_ArvoreB(int t);

//cabeçalho das funções de busca
NoArvB* buscar_no_ArvoreB(NoArvB *No, int k, int *pos);
NoArvB* busca_ArvoreB(ArvoreB *t, int k, int *pos);

//cabeçalho das funções de inserção
//vamos implementar aqui utilizando o método CLRS pois acho ele melhor
//nesse método, ele faz o split antes de inserir. No métoddo tradicional ele insere, vê que está cheio e só ai que ele faz o split
void inserir(ArvoreB *t, int k);
void inserir_com_offset(ArvoreB *t, int k, long off);

//cabeçalho das funções de remoção
//aqui vai ter coisa hein
void remover(ArvoreB *t, int);

//cabeçalho de função de impressão e auxiliares da main
//vou deixar aqui por questão de organizção
void imprimir_arvore(const ArvoreB *t);
void liberar_arvore(ArvoreB *t);

//funções de persistência da árvore (uso dos arquivos binários)
int salvar_ArvoreB_bin(const ArvoreB *t, const char *path);
ArvoreB *carregar_ArvoreB_bin(const char *path);

//funções de range da query
typedef void(*visita_koff_t)(int chave, long off, void *ctx);
void visitar_intervalo(const ArvoreB *t, int a, int b, visita_koff_t cb, void *ctx);

#endif 