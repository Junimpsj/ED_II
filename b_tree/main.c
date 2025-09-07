#include "b_tree.h"
#include <stdio.h>
#include <stdlib.h>

int main() {

    int grau, n, menu;

    printf("Informe o grau minimo da arvore (grau T): ");
    if(scanf("%d", &grau) != 1 || grau < 2) {
        printf("Valor de T invalido!\n");
        printf("T precisa ser >= 2");
        return 1;
    }

    ArvoreB *t = criar_ArvoreB(grau);

do {

    printf("\n=== MENU ARVORE B ===\n");
    printf("[1] - Inserir um valor\n");
    printf("[2] - Buscar um valor\n");
    printf("[3] - Remover um valor\n");
    printf("[4] - Imprimir a arvore\n");
    printf("[0] - Sair do menu\n\n");
    printf("Opcao: ");
    scanf("%d", &menu);

    switch(menu) {

        case 1:
            printf("Quantos valores deseja inserir? ");
                if(scanf("%d", &n) != 1 || n < 0) {
                    printf("Quantidade inválida!");
                    liberar_arvore(t);
                    return 1;
                }

            printf("Digite os valores:\n");
                for(int i = 0 ; i < n ; i++) {
                    int k;
                    printf("[%d] valor: ", i+1);
                    if(scanf("%d", &k) != 1) {
                        printf("Entra inválida!");
                        liberar_arvore(t);
                        return 1;
                    }
                    inserir(t, k);
                }
        break;

        case 2:
        {
            int k, pos = -1;
            printf("Insira a chave para buscar: ");
            if(scanf("%d", &k) != 1) {
                printf("Entrada Inválida!");
                liberar_arvore(t);
                return 1;
            }
            NoArvB *No = busca_ArvoreB(t, k ,&pos);
            if(No) {
                printf("Encontrado: chave %d na posicao %d do no [", k, pos);
                for(int i = 0 ; i < No->n ; i++) {
                    printf("%d%s", No->chave[i], (i + 1 < No->n ? " ":""));
                }
                printf("] (%s)\n", No->folha ? "folha":"interno");
            }else {
                printf("Nao encontrado: %d\n", k);
            }
        }
        break;

        case 3:
            {
                int k;

                printf("Insira a chave para remover: ");
                if (scanf("%d", &k) != 1) {
                    printf("Entrada invalida!\n");
                    liberar_arvore(t);
                    return 1;
                }
                
                remover(t, k);
            }
        break;

        case 4:
            imprimir_arvore(t);
        break;

        case 0:

        break;

        default:
            printf("Opção inválida!");
        break;

    }

}while(menu != 0);



    liberar_arvore(t);
    return 0;
}