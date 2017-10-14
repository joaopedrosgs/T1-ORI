#include <stdio.h>

#include <memory.h>

#define TAMANHO_REGISTRO 100
#define TAMANHO_BLOCO 512

typedef struct {
  char *Nome;
  char *Sobrenome;
}Registro;

 typedef FILE* Arquivo;

 Arquivo CriarArquivo(char *nome) {
   Arquivo arquivo= fopen(nome, "w+");
  if (arquivo == NULL) {
    printf("Problema ao abrir o arquivo");
  }
  char primeiro_bloco[TAMANHO_BLOCO] = {0};
  fwrite(primeiro_bloco, sizeof(char), TAMANHO_BLOCO, arquivo);
  fflush(arquivo);
  return arquivo;
}

void PegarProximoBloco(char bloco[TAMANHO_BLOCO], Arquivo arquivo) {
  int lidos = fread(bloco, sizeof(char), TAMANHO_BLOCO, arquivo);
  if (lidos < TAMANHO_BLOCO) { // Deu ruim, não leu 512 bytes, ou seja, não tem um bloco inteiro pra ler
    int i;
    for (i = 0; i < TAMANHO_BLOCO; i++) bloco[i] = 0;
  }
  fseek(arquivo,-lidos,SEEK_CUR); 
  fwrite(bloco,sizeof(char), TAMANHO_BLOCO,arquivo); // Criando bloco novo
}

void SalvarBloco(char bloco[TAMANHO_BLOCO], Arquivo arquivo) {
  fseek(arquivo, -TAMANHO_BLOCO, SEEK_CUR); // Volta pra salvar em cima
  fwrite(bloco, sizeof(char), TAMANHO_BLOCO, arquivo);
}

int PosicaoParaInsercao(char bloco[TAMANHO_BLOCO]) {
  int i = 0;
  for (i = 0; i + TAMANHO_REGISTRO < TAMANHO_BLOCO; i += TAMANHO_REGISTRO) {
    if (bloco[i] == '*' || bloco[i] == '\0') return i; // * é o caractere pra marcar registros invalidos
  }
  return -1;
}

int InserirRegistro(Arquivo arquivo, Registro r) {
  int res = 0;
  int tamanho_total = strlen(r.Nome) + strlen(r.Sobrenome) + 1; //o +1 é por causa do |, que separa os campos
  if (tamanho_total > TAMANHO_REGISTRO - 2) {
    return res;
  }
  rewind(arquivo);

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
  Arquivo a = CriarArquivo("alaao");
  if (a == NULL) {
    printf("Erro ao abrir o arquivo");
  }
  Registro registros[] = {
      {"Joao Pedro", "Sao gregorio Silva"}, {"Kaua", "Ferreira Costa"},
      {"Lara", "Goncalves Cavalcanti"},     {"Vitória", "Gomes Barros"},
      {"Kaua", "Ferreira Costa"},           {"Lara", "Goncalves Cavalcanti"},
      {"Vitória", "Gomes Barros"}};
  int i = 0;
  int sizeA = sizeof(registros) / sizeof(registros[0]);
  for (i = 0; i < sizeA; i++) {
    if (!InserirRegistro(a, registros[i])) {
      printf("Erro ao inserir");
    }
  }
  fclose(a);
  return 0;
}