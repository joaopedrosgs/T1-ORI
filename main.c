
#include "shared.c"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "bloco.c"
#include "arquivo.c"
#include "registro.c"




int main() {

    Arquivo a = CriarArquivo("dados.txt");
    if (a == NULL) {
        printf("Erro ao abrir o arquivo");
    }
    Registro registros[] = {
            {"Joao Pedro", "Sao gregorio Silva"},
            {"Kaua",       "Ferreira Costa"},
            {"Lara",       "Goncalves Cavalcanti"},
            {"Vitória",    "Gomes Barros"},
            {"Kaua",       "Ferreira Costa"},
            {"baaara",     "Goncalves Cavalcanti"},
            {"dsdteeria",  "Gomes Barros"}};
    int i = 0;
    for (i = 0; i < 1000; i++) {
        if (!InserirRegistro(a, registros[i % 7])) {
            printf("Erro ao inserir");
        }
    }
    char *chave = "Kaua";
    Registro x = BuscaRegistro(a, chave);
    printf("%s", x.Sobrenome);
    fclose(a);
    return 0;
}