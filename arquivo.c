//
// Created by pedro on 24/10/17.
//

#include "arquivo.h"
#include <stdio.h>
#include "bloco.h"
#include "registro.h"
#include "shared.h"

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
// CompactarArquivo substitui o arquivo atual por um novo apenas com registros
// validos
void CompactarArquivo(Arquivo arquivo, char *nome_arquivo) {
  char nome_arquivo_novo[] = "novo_compactado.txt";
  Arquivo novo = CriarArquivo(nome_arquivo_novo);
  rewind(arquivo);
  char bloco[TAMANHO_BLOCO] = {0};
  int n_registro;
  while (PegarProximoBloco(bloco, arquivo) ==
         TAMANHO_BLOCO) {  // Enquanto houver blocos para serem lidos
    for (n_registro = 0; n_registro + TAMANHO_REGISTRO < TAMANHO_BLOCO;
         n_registro = n_registro + TAMANHO_REGISTRO) {  // Para cada registro
      if (*(bloco + n_registro) == '*')                 // Se registro invalido
        continue;
      Registro atual = DecodificarRegistro(
          bloco + n_registro);  // Transforma a string lida em uma struct
      InserirRegistro(novo, atual);
      RemoverRegistroDaMemoria(atual);
    }
  }
  fclose(arquivo);
  remove(nome_arquivo); // remove o arquivo antigo
  rename(nome_arquivo_novo, nome_arquivo); // move o arquivo novo para o lugar do antigo
  arquivo = novo;
}

void FecharArquivo(Arquivo arquivo) {
  CompactarArquivo(arquivo, "dados.txt");
  fclose(arquivo);
}