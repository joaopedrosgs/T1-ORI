//
// Created by pedro on 24/10/17.
//

#include <stdio.h>
#include "bloco.h"

void CriarNovoBloco(FILE *arquivo) {
    char bloco[TAMANHO_BLOCO] = {0};
    fwrite(bloco, sizeof(char), TAMANHO_BLOCO, arquivo); // Criando bloco novo
    fflush(arquivo);
}

int PegarProximoBloco(char bloco[TAMANHO_BLOCO], FILE *arquivo) {
    int lidos = fread(bloco, sizeof(char), TAMANHO_BLOCO, arquivo);
    if (lidos < TAMANHO_BLOCO) { // Deu ruim, não leu 512 bytes, ou seja, não tem
        // um bloco inteiro pra ler
        int i;
        for (i = 0; i < TAMANHO_BLOCO; i++)
            bloco[i] = 0;
    }
    return lidos;
}

void SalvarBloco(char bloco[TAMANHO_BLOCO], FILE *arquivo) {
    fseek(arquivo, -TAMANHO_BLOCO, SEEK_CUR); // Volta pra salvar em cima
    fwrite(bloco, sizeof(char), TAMANHO_BLOCO, arquivo);
    fflush(arquivo);
}