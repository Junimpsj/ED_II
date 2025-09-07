#include "alunos.h"
#include <string.h>

FILE* abrir_alunos(const char *path) {

    FILE *f = fopen(path, "a+b");

    return f;

}
long gravar_aluno(FILE *f, const Aluno *a) {

    if (!f) return -1;

    fseek(f, 0, SEEK_END);

    long off = ftell(f);

    fwrite(a, sizeof(Aluno), 1, f);

    fflush(f);

    return off;
}

int ler_aluno(FILE *f, long off, Aluno *out) {

    if (!f || off < 0) return 0;

    if (fseek(f, off, SEEK_SET) != 0) return 0;
    
    return fread(out, sizeof(Aluno), 1, f) == 1;

}