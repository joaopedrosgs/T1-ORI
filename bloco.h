//
// Created by pedro on 24/10/17.
//

#ifndef T2_ORI_BLOCO_H
#define T2_ORI_BLOCO_H

#include "shared.h"

void CriarNovoBloco(FILE *arquivo);
int PegarProximoBloco(char bloco[TAMANHO_BLOCO], FILE *arquivo);
void SalvarBloco(char bloco[TAMANHO_BLOCO], FILE *arquivo);

#endif //UNTITLED2_BLOCO_H
