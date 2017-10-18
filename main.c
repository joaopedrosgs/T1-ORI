#include <stdio.h>

#include <memory.h>

#define TAMANHO_REGISTRO 100
#define TAMANHO_BLOCO 512

typedef struct {
  char *Nome;
  char *Sobrenome;
} Registro;

typedef FILE *Arquivo;

Arquivo CriarArquivo(char *nome) {
  Arquivo arquivo = fopen(nome, "w+");
  if (arquivo == NULL) {
    printf("Problema ao abrir o arquivo");
  } else {
  CriarNovoBloco(0);
}
  return arquivo;
}

void CriarNovoBloco(int offset){
  char bloco[TAMANHO_BLOCO] = {0};
  int i;
  fseek(arquivo, offset, SEEK_CUR);
  fwrite(bloco, sizeof(char), TAMANHO_BLOCO, arquivo);  // Criando bloco novo
  fflush(arquivo);

}
void PegarProximoBloco(char bloco[TAMANHO_BLOCO], Arquivo arquivo) {
  int lidos = fread(bloco, sizeof(char), TAMANHO_BLOCO, arquivo);
  if (lidos < TAMANHO_BLOCO) {  // Deu ruim, não leu 512 bytes, ou seja, não tem
                                // um bloco inteiro pra ler
    CriarNovoBloco(-lidos);
  }
}

void SalvarBloco(char bloco[TAMANHO_BLOCO], Arquivo arquivo) {
  fseek(arquivo, -TAMANHO_BLOCO, SEEK_CUR);  // Volta pra salvar em cima
  fwrite(bloco, sizeof(char), TAMANHO_BLOCO, arquivo);
  fflush(arquivo);
}

int PosicaoParaInsercao(char bloco[TAMANHO_BLOCO]) {
  int i = 0;
  for (i = 0; i + TAMANHO_REGISTRO < TAMANHO_BLOCO; i += TAMANHO_REGISTRO) {
    if (bloco[i] == '*' || bloco[i] == '\0')
      return i;  // * é o caractere pra marcar registros invalidos
  }
  return -1;
}

int InserirRegistro(Arquivo arquivo, Registro r) {
  int resultado = 0;
  int tamanho_total = strlen(r.Nome) + strlen(r.Sobrenome) + 1;  // o +1 é por causa do |, que separa os campos
  if (tamanho_total > TAMANHO_REGISTRO) {
    return resultado;
  }

  rewind(arquivo);

  char bloco[TAMANHO_BLOCO] = {0};
  int posicao = -1;
  while (posicao < 0) {
    PegarProximoBloco(bloco, arquivo);
    posicao = PosicaoParaInsercao(bloco);
  };

  char buffer[tamanho_total + 1];
  snprintf(buffer, sizeof buffer, "%s|%s", r.Nome, r.Sobrenome);
  int i = 0;
  for (i = 0; i < tamanho_total; i++) {
    bloco[posicao + i] = buffer[i];
  }
  resultado = 1;
  SalvarBloco(bloco, arquivo);
  return resultado;
}

int main() {
  Arquivo a = CriarArquivo("dados.txt");
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
  for (i = 0; i < 10000; i++) {
    if (!InserirRegistro(a, registros[i % 6])) {
      printf("Erro ao inserir");
    }
  }
  fclose(a);
  return 0;
}