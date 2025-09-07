#include "b_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Nessa primeira parte definimos as funções que criam um novo nó para a árvore
//e também uma função que da inicio a nossa árvore, a função cria_ArvoreB

static NoArvB *NovoNo(int t, int folha) { //acabei de aprender aqui que funções static são meio que funções "privadas", só podem ser chamadas dentro desse mesmo arquivo e utilizadas somente aqui

    NoArvB *No = malloc(sizeof(NoArvB));
    
    No->n = 0;
    No->folha = folha; //1 se é folha, 0 se é interno
    No->chave = malloc(sizeof(int) * (2*t-1));
    No->filho = malloc(sizeof(NoArvB*) * (2*t));

    for(int i = 0 ; i < 2*t ; i++) {
        No->filho[i] = NULL;
    }

    return No;
}

ArvoreB *criar_ArvoreB(int t) {

    ArvoreB *T = (ArvoreB*) malloc(sizeof(ArvoreB));

    T->t = t;
    T->raiz = NovoNo(t, 1); //vamos começar com uma única folha vazia

    return T;
}

//agora vamos fazer a função de busca

NoArvB* buscar_no_ArvoreB(NoArvB *No, int k, int *pos) {
    
    //primeira verificação que fazemos, se a árvore estiver vazia retornamos NULL
    if(No == NULL) return NULL;

    int i = 0;
    
    while(i < No->n && k > No->chave[i]) {

        i++;

    }

    if(i < No->n && No->chave[i] ==k) {

        if(pos) {*pos = i;}

        return No;

    }

    if(No->folha) {

        return NULL;

    }

    return buscar_no_ArvoreB(No->filho[i], k, pos);
    
}

//a função está pronta acima, agora vamos utilizar ela em uma função conveniente
//ela é conveniente pois vamos passar a raiz pra função começar a buscar pela raiz
NoArvB* busca_ArvoreB(ArvoreB *t, int k, int *pos) {
    return buscar_no_ArvoreB(t->raiz, k, pos);
}

//agora as funções de inserção

//essa função aqui é o split_child
//ela serve para para dividr um nó cheio em dois, liberando espaço para uma nova inserção
static void split_child(NoArvB *No, int i, int t) {

    NoArvB *y = No->filho[i]; //filho cheio
    NoArvB *z = NovoNo(t, y->folha); //novo irmão à direita

    z->n = t-1;

    for(int j = 0 ; j < t-1 ; j++) {

        z->chave[j] = y->chave[j+t];

    }

    if(!y->folha) {

        for(int j = 0 ; j <t ;j++) {

            z->filho[j] = y->filho[j+t];

        }

    }

    y->n = t-1;

    for(int j = No->n ; j >= i+1 ; j--) {

        No->filho[j+1] = No->filho[j];

    }
    No->filho[i+1] = z;

    for(int j = No->n-1 ; j >= i ; j--) {

        No->chave[j+1] = No->chave[j];

    }
    No->chave[i] = y->chave[t-1];

    No->n += 1;

}