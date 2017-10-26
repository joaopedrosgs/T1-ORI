//
// Created by pedro on 24/10/17.
//

#include <stdio.h>
#include "arquivo.h"
#include "bloco.h"
#include "shared.h"
#include "registro.h"

FILE *CriarArquivo(char *nome) {
    FILE *arquivo = fopen(nome, "w+");
    if (arquivo == NULL) {
        printf("Problema ao abrir o arquivo");
    } else {
        CriarNovoBloco(arquivo);
    }
    return arquivo;
}
FILE *AbrirArquivo(char *nome) {
    FILE *arquivo = fopen(nome, "a+");
    if (arquivo == NULL) {
        printf("Problema ao abrir o arquivo");
    }
    return arquivo;
}
void CompactarArquivo(Arquivo arquivo, char* nome_arquivo) {
    /* TODO
     Algoritmo:
     1) Procurar quantos registros precisam ser removidos e guarde em N
     2) Remover os ultimos N registros não removidos do arquivo
     3) Utilizar o InserirRegistro nesses N registros
     obs: o InserirRegistro já insere em cima de registros já removidos.
    */
    char nome_arquivo_novo[] = "novo_compactado.txt";
    Arquivo novo = CriarArquivo(nome_arquivo_novo);
    rewind(arquivo);
    char bloco[TAMANHO_BLOCO] = {0};
    int n_registro;
    while (PegarProximoBloco(bloco, arquivo) == TAMANHO_BLOCO) {
        for (n_registro = 0;
             n_registro + TAMANHO_REGISTRO < TAMANHO_BLOCO; n_registro = n_registro + TAMANHO_REGISTRO) {
            if (*(bloco + n_registro) == '*')
                continue;
            Registro atual = DecodificarRegistro(bloco + n_registro);
            InserirRegistro(novo, atual);
            RemoverRegistroDaMemoria(atual);
        }
    }
    fclose(arquivo);
    remove(nome_arquivo);
    rename(nome_arquivo_novo, nome_arquivo);
    arquivo = novo;
}

void FecharArquivo(Arquivo arquivo) {
    CompactarArquivo(arquivo,"dados.txt");
    fclose(arquivo);
}