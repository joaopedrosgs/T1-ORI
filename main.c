#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include "arquivo.h"
#include "bloco.h"
#include "registro.h"
#include "shared.h"

int main() {
  setlocale(LC_ALL, "Portuguese");
  Arquivo a = CriarArquivo("dados.txt");
  if (a == NULL) {
    printf("Erro ao abrir o arquivo");
  }
  Registro registros[] = {
      {"Joao Pedro", "Sao gregorio Silva"}, {"Kaua", "Ferreira Costa"},
      {"Lara", "Goncalves Cavalcanti"},     {"Vitória", "Gomes Barros"},
      {"Kaua", "Ferreira Costa"},           {"baaara", "Goncalves Cavalcanti"},
      {"dsdteeria", "Gomes Barros"}};
  int i = 0;
  for (i = 0; i < 20; i++) {
    if (!InserirRegistro(a, registros[i % 7])) {
      printf("Erro ao inserir");
    }
  }

  char *chave = "Kaua";
  Registro x = RemoverRegistro(a, chave);
  char *chave2 = "Joao Pedro";
  x = RemoverRegistro(a, chave2);
  InserirRegistro(a, registros[3]);
  ListarRegistros(a, 0);
  return 0;
}
