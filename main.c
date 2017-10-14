#include <stdio.h>

#include <memory.h>

#define TAMANHO_REGISTRO 100
#define TAMANHO_BLOCO 512

struct Registro {
  char *Nome;
  char *Sobrenome;
};

struct Arquivo {
  FILE *arquivo;
  int atual;
  int bloco_atual;
};

struct Arquivo CriarArquivo(char *nome) {
  struct Arquivo a = {fopen(nome, "w+"), 0, 0};
  if (a.arquivo == NULL) {
    printf("Problema ao abrir o arquivo");
  }
  char primeiro_bloco[TAMANHO_BLOCO] = {0};
  fwrite(primeiro_bloco, sizeof(char), TAMANHO_BLOCO, a.arquivo);
  fflush(a.arquivo);
  return a;
}
void PegarProximoBloco(char bloco[TAMANHO_BLOCO], struct Arquivo *arquivo) {
  int lidos = fread(bloco, sizeof(char), TAMANHO_BLOCO, arquivo->arquivo);
  if (lidos < 512) {
    int i;
    for (i = 0; i < TAMANHO_BLOCO; i++) bloco[i] = 0;
  }
  fseek(arquivo->arquivo,-lidos,SEEK_CUR);
  fwrite(bloco,sizeof(char), TAMANHO_BLOCO,arquivo->arquivo);
}

int PosicaoParaInsercao(char bloco[TAMANHO_BLOCO]) {
  int i = 0;
  for (i = 0; i + TAMANHO_REGISTRO < TAMANHO_BLOCO; i += TAMANHO_REGISTRO) {
    if (bloco[i] == '*' || bloco[i] == '\0') return i;
  }
  return -1;
}
void SalvarBloco(char bloco[TAMANHO_BLOCO], struct Arquivo *arquivo) {
  fseek(arquivo->arquivo, -TAMANHO_BLOCO, SEEK_CUR);
  fwrite(bloco, sizeof(char), TAMANHO_BLOCO, arquivo->arquivo);
}
int InserirRegistro(struct Arquivo *arquivo, struct Registro r) {
  int res = 0;
  int tamanho_total = strlen(r.Nome) + strlen(r.Sobrenome) + 1;
  if (tamanho_total > TAMANHO_REGISTRO - 2) {
    return res;
  }
  rewind(arquivo->arquivo);

  char bloco[TAMANHO_BLOCO] = {0};
  int posicao = -1;
  while (posicao < 0) {
    PegarProximoBloco(bloco, arquivo);
    posicao = PosicaoParaInsercao(bloco);
  };

  char buf[tamanho_total + 1];
  snprintf(buf, sizeof buf, "%s|%s", r.Nome, r.Sobrenome);
  int i = 0;
  for (i = 0; i < tamanho_total; i++) {
    bloco[posicao + i] = buf[i];
  }
  res = 1;
  SalvarBloco(bloco, arquivo);
  return res;
}

int main() {
  struct Arquivo b = CriarArquivo("alaao");
  if (b.arquivo == NULL) {
    printf("erro");
  }
  struct Registro a[] = {
      {"Joao Pedro", "Sao gregorio Silva"}, {"Kaua", "Ferreira Costa"},
      {"Lara", "Goncalves Cavalcanti"},     {"Vitória", "Gomes Barros"},
      {"Kaua", "Ferreira Costa"},           {"Lara", "Goncalves Cavalcanti"},
      {"Vitória", "Gomes Barros"}};
  int i = 0;
  int sizeA = sizeof(a) / sizeof(a[0]);
  for (i = 0; i < sizeA; i++) {
    if (!InserirRegistro(&b, a[i])) {
      printf("Erro ao inserir");
    }
  }
  fclose(b.arquivo);
  return 0;
}