//
// Created by pedro on 24/10/17.
//

#include "bloco.h"
#include <stdio.h>

void CriarNovoBloco(FILE *arquivo) {
  char bloco[TAMANHO_BLOCO] = {0};
  fwrite(bloco, sizeof(char), TAMANHO_BLOCO, arquivo);  // Criando bloco novo
  fflush(arquivo);
}
//Pega o proximo bloco no arquivo
int PegarProximoBloco(char bloco[TAMANHO_BLOCO], FILE *arquivo) {
  int lidos = fread(bloco, sizeof(char), TAMANHO_BLOCO, arquivo);
  if (lidos != TAMANHO_BLOCO) {  // Não conseguiu ler o bloco inteiro
    int i;
    for (i = 0; i < TAMANHO_BLOCO; i++)  // Zera o bloco por conveniencia
      bloco[i] = 0;
  }
  return lidos;
}
// Salva o ultimo bloco lido (Leva em consideração que tem de voltar para salvar)
void SalvarBloco(char bloco[TAMANHO_BLOCO], FILE *arquivo) {
  fseek(arquivo, -TAMANHO_BLOCO, SEEK_CUR);  // Volta pra salvar em cima
  fwrite(bloco, sizeof(char), TAMANHO_BLOCO, arquivo);
  fflush(arquivo);
}