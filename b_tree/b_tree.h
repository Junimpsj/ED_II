#ifndef B_TREE_H
#define B_TREE_H

#define MAX 5

typedef struct NoArvB {
    int n;
    int folha;
    int *chave; //isso aqui é no máximo 2t-1
    struct NoArvB **filho; //isso aqui é 2t, tem um apontador a mais pros filhos
} NoArvB;

typedef struct {
    int t;
    NoArvB *raiz;
} ArvoreB;

//cabeçalho das funções de criar um novo nó e criar a árvore
static NoArvB *NovoNo(int t, int folha);
ArvoreB *criar_ArvoreB(int t);

//cabeçalho das funções de busca
NoArvB* buscar_no_ArvoreB(NoArvB *No, int k, int *pos);
NoArvB* busca_ArvoreB(ArvoreB *t, int k, int *pos);

//cabeçalho das funções de inserção
//vamos implementar aqui utilizando o método CLRS pois acho ele melhor
//nesse método, ele faz o split antes de inserir. No métoddo tradicional ele insere, vê que está cheio e só ai que ele faz o split
static void split_child(NoArvB *No, int i, int t); //esse é o split child, a função auxilar que divide um nó filho que está cheio para possibilitar uma inserção
static void inserir_naocheio(NoArvB *No, int k, int t);
void inserir(ArvoreB *t, int k);


#endif 