//
// Created by pedro on 24/10/17.
//

#include <stdio.h>
#include "arquivo.h"

typedef FILE *Arquivo;

FILE *CriarArquivo(char *nome) {
    FILE *arquivo = fopen(nome, "w+");
    if (arquivo == NULL) {
        printf("Problema ao abrir o arquivo");
    } else {
        CriarNovoBloco(arquivo);
    }
    return arquivo;
}

void CompactarArquivo(Arquivo arquivo) {
    /* TODO
     Algoritmo:
     1) Procurar quantos registros precisam ser removidos e guarde em N
     2) Remover os ultimos N registros não removidos do arquivo
     3) Utilizar o InserirRegistro nesses N registros
     obs: o InserirRegistro já insere em cima de registros já removidos.
    */}

void FecharArquivo(Arquivo arquivo) {
    CompactarArquivo(arquivo);
    fclose(arquivo);
}