#ifndef ALUNOS_H
#define ALUNOS_H
#include <stdio.h>

typedef struct {
    int  ra;
    char nome[64];
    char email[64];
} Aluno;

//abre em append+read binário (cria se não existir)
FILE* abrir_alunos(const char *path);

//grava no final e retorna offset
long gravar_aluno(FILE *f, const Aluno *a);

//lê aluno a partir do offset salvo
int ler_aluno(FILE *f, long off, Aluno *out);

#endif