#ifndef T2_ORI_REGISTRO_H
#define T2_ORI_REGISTRO_H

#include "arquivo.h"
#include "shared.h"

typedef struct {
  char *Nome;
  char *Sobrenome;
} Registro;

int PosicaoParaInsercao(char bloco[TAMANHO_BLOCO]);

int InserirRegistro(FILE *arquivo, Registro r);
Registro BuscaRegistro(Arquivo arquivo, char *chave);
Registro RemoverRegistro(Arquivo arquivo, char *chave);

int EncontrarIndice(Arquivo arquivo, char *chave, int tamanho_chave);

Registro DecodificarRegistro(char *texto);
char *CodificarRegistro(Registro r);

void RemoverRegistroDaMemoria(Registro r);

void ListarRegistros(Arquivo arquivo, int imprimir_deletados);

#endif  // T2_ORI_REGISTRO_H
