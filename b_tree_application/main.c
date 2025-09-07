#include "b_tree.h"
#include "alunos.h"
#include <stdio.h>
#include <stdlib.h>

static void print_aluno_cb(int chave, long off, void *ctx) {

    FILE *f = (FILE*)ctx;

    if (!f || off < 0) return;

    Aluno a;

    if (ler_aluno(f, off, &a)) {

        printf("RA=%d | Nome=%s | Email=%s (off=%ld)\n", a.ra, a.nome, a.email, off);

    }

}

int main() {

    int grau, menu;

    printf("Informe o grau minimo da arvore (grau T): ");
    if (scanf("%d", &grau) != 1 || grau < 2) {
        printf("Valor de T invalido! (T >= 2)\n");
        return 1;
    }

    ArvoreB *t = criar_ArvoreB(grau);

    FILE *fAlunos = abrir_alunos("alunos.dat");

    if (!fAlunos) { printf("Erro abrindo alunos.dat\n"); return 1; }

    do {
        printf("\n=== INDICE DE ALUNOS POR RA (ArvoreB) ===\n");
        printf("[1] - Inserir aluno (RA, nome, email)\n");
        printf("[2] - Buscar aluno por RA\n");
        printf("[3] - Remover RA\n");
        printf("[4] - Listar por faixa de RA\n");
        printf("[5] - Imprimir arvore\n");
        printf("[6] - Salvar indice em indice_ra.bin\n");
        printf("[7] - Carregar indice de indice_ra.bin\n");
        printf("[0] - Sair\n");
        printf("\nOpcao: ");
        if (scanf("%d", &menu) != 1) break;

       switch(menu) {

        case 1:
            {

            Aluno a;

            printf("RA: ");    scanf("%d", &a.ra);
            printf("Nome: ");  scanf(" %63[^\n]", a.nome);
            printf("Email: "); scanf(" %63s", a.email);

            long off = gravar_aluno(fAlunos, &a);

            if (off >= 0) {
                inserir_com_offset(t, a.ra, off);
                printf("Inserido RA=%d (offset=%ld)\n", a.ra, off);
            } else {
                printf("Falha ao gravar aluno\n");
            }

            }
        break;

        case 2:
            {

            int ra, pos=-1;

            printf("RA para buscar: "); scanf("%d", &ra);

            NoArvB *No = busca_ArvoreB(t, ra, &pos);

            if (No && pos >= 0) {
                Aluno a;
                if (ler_aluno(fAlunos, No->offset[pos], &a))
                    printf("Encontrado: RA=%d | Nome=%s | Email=%s\n", a.ra, a.nome, a.email);
                else
                    printf("Offset inválido no índice.\n");
            } else {
                printf("RA %d não encontrado.\n", ra);
            }

            }
        break;

        case 3:
            {

            int ra; printf("RA para remover: "); scanf("%d", &ra);
            remover(t, ra);
            printf("Removido (se existia)\nOBS: Registro bruto permanece em alunos.dat\n");

            }
        break;

        case 4:
            {
            int a,b; printf("RA inicial: "); scanf("%d",&a);
            printf("RA final: ");   scanf("%d",&b);
            visitar_intervalo(t, a, b, print_aluno_cb, fAlunos);
            }
        break;

        case 5:
            imprimir_arvore(t);
        break;

        case 6:
            if (salvar_ArvoreB_bin(t, "indice_ra.bin")) printf("Indice salvo\n");
            else printf("Falha ao salvar indice\n");
        break;

        case 7:
            {
            ArvoreB *novo = carregar_ArvoreB_bin("indice_ra.bin");
            if (novo) {
                liberar_arvore(t); 
                t = novo; printf("Indice carregado\n"); 
            }
            else printf("Falha ao carregar indice\n");
            }
        break;

        case 0:

        break;

        default:
            printf("Opcao Invalida!");
        break;

       }

    } while (menu != 0);

    fclose(fAlunos);
    liberar_arvore(t);
    return 0;

}