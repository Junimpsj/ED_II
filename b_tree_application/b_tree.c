#include "b_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//função auxiliar para criar novo nó
static NoArvB *NovoNo(int t, int folha) {

    NoArvB *No = (NoArvB*) malloc(sizeof(NoArvB));
    if (!No) exit(1);
    No->n = 0;
    No->folha = folha;
    No->chave  = (int*)  malloc(sizeof(int)  * (2*t - 1));
    No->offset = (long*) malloc(sizeof(long) * (2*t - 1));
    No->filho  = (NoArvB**) malloc(sizeof(NoArvB*) * (2*t));
    if (!No->chave || !No->offset || !No->filho) exit(1);
    for (int i = 0; i < 2*t; i++) No->filho[i] = NULL;
    for (int i = 0; i < 2*t-1; i++) No->offset[i] = 0L;
    return No;

}

//função para liberar um nó e seus filhos recursivamente
static void liberar_no(NoArvB *No) {

    if (!No) return;
    if (!No->folha) {
        for (int i = 0; i <= No->n; i++) {
            liberar_no(No->filho[i]);
        }
    }
    free(No->chave);
    free(No->offset);
    free(No->filho);
    free(No);

}

//funções públicas de criação e liberação
ArvoreB *criar_ArvoreB(int t) {

    ArvoreB *T = (ArvoreB*) malloc(sizeof(ArvoreB));
    if (!T) exit(1);
    T->t = t;
    T->raiz = NovoNo(t, 1);
    return T;

}

void liberar_arvore(ArvoreB *t) {

    if (!t) return;
    liberar_no(t->raiz);
    free(t);

}

//funções públicas de busca
NoArvB* buscar_no_ArvoreB(NoArvB *No, int k, int *pos) {

    if (No == NULL) return NULL;
    int i = 0;
    while (i < No->n && k > No->chave[i]) i++;
    if (i < No->n && No->chave[i] == k) { if (pos) *pos = i; return No; }
    if (No->folha) return NULL;
    return buscar_no_ArvoreB(No->filho[i], k, pos);

}

NoArvB* busca_ArvoreB(ArvoreB *t, int k, int *pos) {

    return buscar_no_ArvoreB(t->raiz, k, pos);

}

//funções auxiliares para inserção
static void split_child(NoArvB *No, int i, int t) {

    NoArvB *y = No->filho[i];
    NoArvB *z = NovoNo(t, y->folha);

    z->n = t - 1;

    for (int j = 0; j < t - 1; j++) {
        z->chave[j] = y->chave[j + t];
        z->offset[j] = y->offset[j + t];
    }
    
    if (!y->folha) {
        for (int j = 0; j < t; j++) {
            z->filho[j] = y->filho[j + t];
        }
    }

    int medK   = y->chave[t - 1];
    long medOf = y->offset[t - 1];
    y->n = t - 1;

    for (int j = No->n; j >= i + 1; j--) No->filho[j + 1] = No->filho[j];
    No->filho[i + 1] = z;

    for (int j = No->n - 1; j >= i; j--) {
        No->chave[j + 1] = No->chave[j];
        No->offset[j + 1] = No->offset[j];
    }
    No->chave[i]  = medK;
    No->offset[i] = medOf;

    No->n += 1;

}

static void inserir_naocheio_com_off(NoArvB *No, int k, long off, int t) {

    int i = No->n - 1;

    if (No->folha) {
        while (i >= 0 && k < No->chave[i]) {
            No->chave[i + 1] = No->chave[i];
            No->offset[i + 1] = No->offset[i];
            i--;
        }
        No->chave[i + 1] = k;
        No->offset[i + 1] = off;
        No->n += 1;
        return;
    }

    while (i >= 0 && k < No->chave[i]) i--;
    i++;

    if (No->filho[i]->n == 2*t - 1) {
        split_child(No, i, t);
        if (k > No->chave[i]) i++;
    }
    inserir_naocheio_com_off(No->filho[i], k, off, t);

}

//funções públicas de inserção
void inserir_com_offset(ArvoreB *t, int k, long off) {

    NoArvB *r = t->raiz; int ordem = t->t;
    if (r->n == 2*ordem - 1) {
        NoArvB *s = NovoNo(ordem, 0);
        t->raiz = s;
        s->filho[0] = r;
        split_child(s, 0, ordem);
        inserir_naocheio_com_off(s, k, off, ordem);
    } else {
        inserir_naocheio_com_off(r, k, off, ordem);
    }

}

void inserir(ArvoreB *t, int k) {

    inserir_com_offset(t, k, 0L);

}

//funções auxiliares para remoção
static void obter_predecessor_koff(NoArvB *x, int i, int *outK, long *outOff) {

    NoArvB *atual = x->filho[i];
    while (!atual->folha) atual = atual->filho[atual->n];
    *outK = atual->chave[atual->n - 1];
    *outOff = atual->offset[atual->n - 1];

}

static void obter_sucessor_koff(NoArvB *x, int i, int *outK, long *outOff) {

    NoArvB *atual = x->filho[i + 1];
    while (!atual->folha) atual = atual->filho[0];
    *outK = atual->chave[0];
    *outOff = atual->offset[0];

}

static void fundir_filhos(NoArvB *x, int i, int t) {

    NoArvB *filho = x->filho[i];
    NoArvB *irmao = x->filho[i + 1];

    filho->chave[t - 1] = x->chave[i];
    filho->offset[t - 1] = x->offset[i];

    for (int j = 0; j < irmao->n; j++) {
        filho->chave[j + t] = irmao->chave[j];
        filho->offset[j + t] = irmao->offset[j];
    }
    
    if (!filho->folha) {
        for (int j = 0; j <= irmao->n; j++) {
            filho->filho[j + t] = irmao->filho[j];
        }
    }

    for (int j = i + 1; j < x->n; j++) {
        x->chave[j - 1] = x->chave[j];
        x->offset[j - 1] = x->offset[j];
        x->filho[j] = x->filho[j + 1];
    }

    filho->n += irmao->n + 1;
    x->n--;

    free(irmao->chave);
    free(irmao->offset);
    free(irmao->filho);
    free(irmao);

}

static void emprestar_do_irmao_esq(NoArvB *x, int i) {

    NoArvB *filho = x->filho[i];
    NoArvB *irmao = x->filho[i - 1];

    for (int j = filho->n - 1; j >= 0; j--) {
        filho->chave[j + 1] = filho->chave[j];
        filho->offset[j + 1] = filho->offset[j];
    }
    
    if (!filho->folha) {
        for (int j = filho->n; j >= 0; j--) {
            filho->filho[j + 1] = filho->filho[j];
        }
    }
    
    filho->chave[0] = x->chave[i - 1];
    filho->offset[0] = x->offset[i - 1];
    
    if (!filho->folha) {
        filho->filho[0] = irmao->filho[irmao->n];
    }

    x->chave[i - 1] = irmao->chave[irmao->n - 1];
    x->offset[i - 1] = irmao->offset[irmao->n - 1];

    filho->n += 1;
    irmao->n -= 1;

}

static void emprestar_do_irmao_dir(NoArvB *x, int i) {

    NoArvB *filho = x->filho[i];
    NoArvB *irmao = x->filho[i + 1];

    filho->chave[filho->n] = x->chave[i];
    filho->offset[filho->n] = x->offset[i];
    
    if (!filho->folha) {
        filho->filho[filho->n + 1] = irmao->filho[0];
    }

    x->chave[i] = irmao->chave[0];
    x->offset[i] = irmao->offset[0];

    for (int j = 1; j < irmao->n; j++) {
        irmao->chave[j - 1] = irmao->chave[j];
        irmao->offset[j - 1] = irmao->offset[j];
    }
    
    if (!irmao->folha) {
        for (int j = 1; j <= irmao->n; j++) {
            irmao->filho[j - 1] = irmao->filho[j];
        }
    }

    filho->n += 1;
    irmao->n -= 1;

}

static void preencher(NoArvB *x, int i, int t) {

    if (i != 0 && x->filho[i - 1]->n >= t) {
        emprestar_do_irmao_esq(x, i);
    } else if (i != x->n && x->filho[i + 1]->n >= t) {
        emprestar_do_irmao_dir(x, i);
    } else {
        if (i != x->n) {
            fundir_filhos(x, i, t);
        } else {
            fundir_filhos(x, i - 1, t);
        }
    }

}

static void remover_chave(NoArvB *x, int k, int t) {

    int idx = 0;
    while (idx < x->n && x->chave[idx] < k) idx++;

    if (idx < x->n && x->chave[idx] == k) {
        if (x->folha) {
            for (int i = idx + 1; i < x->n; i++) {
                x->chave[i - 1] = x->chave[i];
                x->offset[i - 1] = x->offset[i];
            }
            x->n -= 1;
        } else {
            int k_subst;
            long off_subst;
            
            if (x->filho[idx]->n >= t) {
                obter_predecessor_koff(x, idx, &k_subst, &off_subst);
                x->chave[idx] = k_subst;
                x->offset[idx] = off_subst;
                remover_chave(x->filho[idx], k_subst, t);
            } else if (x->filho[idx + 1]->n >= t) {
                obter_sucessor_koff(x, idx, &k_subst, &off_subst);
                x->chave[idx] = k_subst;
                x->offset[idx] = off_subst;
                remover_chave(x->filho[idx + 1], k_subst, t);
            } else {
                fundir_filhos(x, idx, t);
                remover_chave(x->filho[idx], k, t);
            }
        }
    } else {
        if (x->folha) return;
        int flag = (idx == x->n);
        if (x->filho[idx]->n < t) preencher(x, idx, t);
        if (flag && idx > x->n) {
            remover_chave(x->filho[idx - 1], k, t);
        } else {
            remover_chave(x->filho[idx], k, t);
        }
    }

}

//função pública de remoção
void remover(ArvoreB *T, int k) {

    if (!T || !T->raiz) return;
    remover_chave(T->raiz, k, T->t);
    if (T->raiz->n == 0) {
        NoArvB *antigaRaiz = T->raiz;
        T->raiz = T->raiz->folha ? NULL : T->raiz->filho[0];
        liberar_no(antigaRaiz);
    }

}

//funções auxiliares para persistência
static int salvar_no(FILE *fp, const NoArvB *No, int t) {

    if (!No) {
        int vazio = 0;
        fwrite(&vazio, sizeof(int), 1, fp);
        return 1;
    }

    int existe = 1;
    fwrite(&existe, sizeof(int), 1, fp);
    fwrite(&No->n, sizeof(int), 1, fp);
    fwrite(&No->folha, sizeof(int), 1, fp);
    fwrite(No->chave, sizeof(int), No->n, fp);
    fwrite(No->offset, sizeof(long), No->n, fp);

    for (int i = 0; i <= No->n; i++) {
        if (!salvar_no(fp, No->filho[i], t)) return 0;
    }

    return 1;

}

static NoArvB *carregar_no(FILE *fp, int t) {

    int existe;
    fread(&existe, sizeof(int), 1, fp);
    if (!existe) return NULL;

    NoArvB *No = (NoArvB *)malloc(sizeof(NoArvB));
    if (!No) return NULL;

    No->chave = (int *)malloc(sizeof(int) * (2 * t - 1));
    No->offset = (long *)malloc(sizeof(long) * (2 * t - 1));
    No->filho = (NoArvB **)malloc(sizeof(NoArvB *) * (2 * t));

    fread(&No->n, sizeof(int), 1, fp);
    fread(&No->folha, sizeof(int), 1, fp);
    fread(No->chave, sizeof(int), No->n, fp);
    fread(No->offset, sizeof(long), No->n, fp);

    for (int i = 0; i <= No->n; i++) {
        No->filho[i] = carregar_no(fp, t);
    }

    return No;

}

//funções públicas de persistência
int salvar_ArvoreB_bin(const ArvoreB *t, const char *path) {

    if (!t || !path) return 0;
    FILE *fp = fopen(path, "wb");
    if (!fp) return 0;
    fwrite(&t->t, sizeof(int), 1, fp);
    int sucesso = salvar_no(fp, t->raiz, t->t);
    fclose(fp);
    return sucesso;

}

ArvoreB *carregar_ArvoreB_bin(const char *path) {

    if (!path) return NULL;
    FILE *fp = fopen(path, "rb");
    if (!fp) return NULL;
    int t;
    fread(&t, sizeof(int), 1, fp);
    ArvoreB *T = (ArvoreB *)malloc(sizeof(ArvoreB));
    if (!T) {
        fclose(fp);
        return NULL;
    }
    T->t = t;
    T->raiz = carregar_no(fp, t);
    fclose(fp);
    return T;

}

//funções auxiliares para visualização
static void imprimir_no(const NoArvB *No, int nivel) {

    if (!No) return;
    for (int i = 0; i < nivel; i++) printf("  ");
    printf("[");
    for (int i = 0; i < No->n; i++) {
        printf("%d", No->chave[i]);
        if (i + 1 < No->n) printf(", ");
    }
    printf("]\n");
    if (!No->folha) {
        for (int i = 0; i <= No->n; i++) {
            imprimir_no(No->filho[i], nivel + 1);
        }
    }

}

//função pública para impressão da árvore
void imprimir_arvore(const ArvoreB *t) {

    if (!t || !t->raiz) {
        printf("Árvore Vazia\n");
        return;
    }
    imprimir_no(t->raiz, 0);

}

//funções para intervalos de busca
static void visitar_intervalo_no(const NoArvB *No, int a, int b, visita_koff_t cb, void *ctx) {

    if (!No) return;
    int i = 0;
    while (i < No->n && No->chave[i] < a) i++;
    while (i < No->n && No->chave[i] <= b) {
        cb(No->chave[i], No->offset[i], ctx);
        i++;
    }
    if (!No->folha) {
        i = 0;
        while (i <= No->n) {
            if (i == 0 || (i < No->n && No->chave[i-1] < b)) {
                if (i == No->n || No->chave[i] > a) {
                    visitar_intervalo_no(No->filho[i], a, b, cb, ctx);
                }
            }
            i++;
        }
    }

}

//função pública para visitar intervalo
void visitar_intervalo(const ArvoreB *t, int a, int b, visita_koff_t cb, void *ctx) {

    if (!t || !t->raiz) return;
    visitar_intervalo_no(t->raiz, a, b, cb, ctx);

}
