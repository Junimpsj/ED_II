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

//funções de busca

NoArvB* buscar_no_ArvoreB(NoArvB *No, int k, int *pos) {
    
    //primeira verificação que fazemos, se a árvore estiver vazia retornamos NULL
    if(No == NULL) return NULL;

    int i = 0;
    
    while(i < No->n && k > No->chave[i]) {

        i++;

    }

    if(i < No->n && No->chave[i] ==k) {

        if(pos) {*pos = i;} //se a chave é achada, então existe uma posição, e ela é gravada aqui

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

//funções de inserção

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

static void inserir_naocheio(NoArvB *No, int k, int t) {

    int i = No->n - 1;

    if(No->folha) {

        while(i >= 0 && k < No->chave[i]) {

            No->chave[i+1] = No->chave[i];
            i--;

        }

        No->chave[i+1] = k;
        No->n += 1;
        return;

    }

    while(i >= 0 && k < No->chave[i]) {i--;}
        i++;

        if(No->filho[i]->n == 2*t-1) {

            split_child(No, i, t);

            if (k > No->chave[i]) {i++;}

        }

            inserir_naocheio(No->filho[i], k , t);

}

void inserir(ArvoreB *t, int k) {

    NoArvB *r = t->raiz;
    int ordem = t->t; //aqui eu declarei o "t" como "ordem" pq eu tive um probleminha rs

    if(r->n == 2*ordem-1) {

        NoArvB *s = NovoNo(ordem, 0);
        t->raiz = s;
        s->filho[0] = r;
        s->n = 0;

        split_child(s, 0, ordem);

        int i = (k > s->chave[0]) ? 1:0;
        inserir_naocheio(s->filho[i], k, ordem);

    }else {

        inserir_naocheio(r, k, ordem);

    }

}

//funções de remoção

static int obter_predecessor(NoArvB *x, int i) {

    NoArvB *atual = x->filho[i];

    while (!atual->folha) {

        atual = atual->filho[atual->n]; //desce sempre pelo último filho

    }

    return atual->chave[atual->n - 1];

}

static int obter_sucessor(NoArvB *x, int i) {

    NoArvB *atual = x->filho[i + 1];

    while (!atual->folha) {

        atual = atual->filho[0]; //desce sempre pelo primeiro filho

    }

    return atual->chave[0];

}

static void fundir_filhos(NoArvB *x, int i, int t) {

    NoArvB *y = x->filho[i];
    NoArvB *z = x->filho[i + 1];

    y->chave[y->n] = x->chave[i];
    y->n += 1;

    for (int j = 0; j < z->n; j++) {

        y->chave[y->n + j] = z->chave[j];

    }

    //se não é folha copia ponteiros de filhos
    if (!y->folha) {

        for (int j = 0; j <= z->n; j++) {
            y->filho[y->n + j] = z->filho[j];
        }

    }
    y->n += z->n;

    for (int j = i; j < x->n - 1; j++) {
        x->chave[j] = x->chave[j + 1];
        x->filho[j + 1] = x->filho[j + 2];
    }

    x->n -= 1;

    free(z->chave);
    free(z->filho);
    free(z);

}

static void emprestar_do_irmao_esq(NoArvB *x, int i) {

    NoArvB *child = x->filho[i];
    NoArvB *irmao = x->filho[i - 1];

    for (int j = child->n - 1; j >= 0; j--) {
        child->chave[j + 1] = child->chave[j];
    }

    if (!child->folha) {

        for (int j = child->n; j >= 0; j--) {
            child->filho[j + 1] = child->filho[j];
        }

    }

    child->chave[0] = x->chave[i - 1];

    if (!child->folha) {

        child->filho[0] = irmao->filho[irmao->n];

    }

    child->n += 1;

    x->chave[i - 1] = irmao->chave[irmao->n - 1];
    irmao->n -= 1;

}

static void emprestar_do_irmao_dir(NoArvB *x, int i) {

    NoArvB *child = x->filho[i];
    NoArvB *irmao = x->filho[i + 1];

    child->chave[child->n] = x->chave[i];

    if (!child->folha) {

        child->filho[child->n + 1] = irmao->filho[0];

    }

    child->n += 1;

    x->chave[i] = irmao->chave[0];

    for (int j = 1; j < irmao->n; j++) {
        irmao->chave[j - 1] = irmao->chave[j];
    }

    if (!irmao->folha) {

        for (int j = 1; j <= irmao->n; j++) {
            irmao->filho[j - 1] = irmao->filho[j];
        }

    }

    irmao->n -= 1;

}

static void preencher(NoArvB *x, int i, int t) {

    if (i > 0 && x->filho[i - 1]->n >= t) {

        //caso 3(a): empresta do irmão esquerdo
        emprestar_do_irmao_esq(x, i);

    } else if (i < x->n && x->filho[i + 1]->n >= t) {

        //caso 3(a): empresta do irmão direito
        emprestar_do_irmao_dir(x, i);

    } else {

        //caso 3(b): merge com um irmão (se existir é melhor unir com o irmão esquerdo)
        if (i < x->n) {

            fundir_filhos(x, i, t);

        } else {

            fundir_filhos(x, i - 1, t);

        }
    }

}

static void remover_de_folha(NoArvB *x, int idx) {

    for (int i = idx + 1; i < x->n; i++) {
        x->chave[i - 1] = x->chave[i];
    }
    x->n -= 1;

}

static void remover_de_interno(NoArvB *x, int idx, int t) {

    int k = x->chave[idx];

    NoArvB *y = x->filho[idx];
    NoArvB *z = x->filho[idx + 1];

    if (y->n >= t) {

        //caso 2(a): usa predecessor
        int pred = obter_predecessor(x, idx);
        x->chave[idx] = pred;
        extern void remover_chave(NoArvB *x, int k, int t);
        remover_chave(y, pred, t);

    } else if (z->n >= t) {

        //caso 2(b): usa sucessor
        int succ = obter_sucessor(x, idx);
        x->chave[idx] = succ;
        extern void remover_chave(NoArvB *x, int k, int t);
        remover_chave(z, succ, t);

    } else {

        //caso 2(c): merge k em y com z, depois remove k em y
        fundir_filhos(x, idx, t);
        extern void remover_chave(NoArvB *x, int k, int t);
        remover_chave(y, k, t);

    }

}

void remover_chave(NoArvB *x, int k, int t) {

    int idx = 0;

    while (idx < x->n && x->chave[idx] < k) idx++;

    if (idx < x->n && x->chave[idx] == k) {

        if (x->folha) {

            //caso 1
            remover_de_folha(x, idx);

        } else {

            //caso 2a/2b/2c
            remover_de_interno(x, idx, t);

        }

        return;
    }

    if (x->folha) return;


    int i = idx;

    if (x->filho[i]->n < t) {

        preencher(x, i, t);

        if (i > x->n) i = x->n;

    }

    //agora desce recursivamente
    remover_chave(x->filho[i], k, t);

}

void remover(ArvoreB *T, int k) {

    if (!T || !T->raiz) return;

    NoArvB *r = T->raiz;

    int t = T->t;

    remover_chave(r, k, t);

    if (r->n == 0 && !r->folha) {

        NoArvB *novoR = r->filho[0];
        //libera a antiga raiz
        free(r->chave);
        free(r->filho);
        free(r);
        T->raiz = novoR;

    }

}

//funções de impressão e auxiliares da main
void imprimir_no(const NoArvB *No, int nivel) {

    if(!No) return;
    
    for(int s = 0 ; s < nivel*2 ; s++) printf(" ");

    printf("[");
    for(int i = 0 ; i < No->n ; i++) {

        printf("%d", No->chave[i]);
        if(i+1 < No->n) printf(" ");

    }
    printf("]\n");

    if(!No->folha) {

        for(int i = 0 ; i <= No->n ; i++) {
            
            imprimir_no(No->filho[i], nivel+1);

        }

    }

}

void imprimir_arvore(const ArvoreB *t) {

    if(!t || !t->raiz) {

        printf("Árvore Vazia");
        return;

    }
    imprimir_no(t->raiz, 0);

}

static void liberar_no(NoArvB *No, int t) {

    if(!No) return;

    if(!No->folha) {

        for(int i = 0 ; i <= No->n ; i++){
            liberar_no(No->filho[i], t);
        }

    }

    free(No->chave);
    free(No->filho);
    free(No);

}

void liberar_arvore(ArvoreB *t) {

    if(!t) return;
    liberar_no(t->raiz, t->t);
    free(t);
    
}