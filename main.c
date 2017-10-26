#include "shared.h"
#include <stdio.h>
#include <stdlib.h>
#include "arquivo.h"
#include "registro.h"
#include "bloco.h"
#include <locale.h>


int main() {
    setlocale(LC_ALL, "Portuguese");
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
    for (i = 0; i < 20; i++) {
        if (!InserirRegistro(a, registros[i % 7])) {
            printf("Erro ao inserir");
        }
    }
    char *chave = "Kaua";
    Registro x = RemoverRegistro(a, chave);
    free(x.Nome);
    free(x.Sobrenome);
    char *chave2 = "Joao Pedro";
    x = RemoverRegistro(a, chave2);
    ListarRegistros(a, 1);
    FecharArquivo(a);
    a = AbrirArquivo("dados.txt");
    ListarRegistros(a, 1);
    return 0;
}

