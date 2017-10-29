#ifndef T2_ORI_BLOCO_H
#define T2_ORI_BLOCO_H
#include <stdio.h>
#include "shared.h"

void CriarNovoBloco(FILE *arquivo);
int PegarProximoBloco(char bloco[TAMANHO_BLOCO], FILE *arquivo);
void SalvarBloco(char bloco[TAMANHO_BLOCO], FILE *arquivo);

#endif  // T2_ORI_BLOCO_H
