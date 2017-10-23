#include <stdio.h>

#include <memory.h>

#define TAMANHO_REGISTRO 100
#define TAMANHO_BLOCO 512

typedef struct {
  char *Nome;
  char *Sobrenome;
} Registro;

typedef FILE *Arquivo;
void CriarNovoBloco(Arquivo arquivo) {
  char bloco[TAMANHO_BLOCO] = {0};
  fwrite(bloco, sizeof(char), TAMANHO_BLOCO, arquivo);  // Criando bloco novo
  fflush(arquivo);
}
Arquivo CriarArquivo(char *nome) {
  Arquivo arquivo = fopen(nome, "w+");
  if (arquivo == NULL) {
    printf("Problema ao abrir o arquivo");
  } else {
    CriarNovoBloco(arquivo);
  }
  return arquivo;
}

void PegarProximoBloco(char bloco[TAMANHO_BLOCO], Arquivo arquivo) {
  int lidos = fread(bloco, sizeof(char), TAMANHO_BLOCO, arquivo);
  if (lidos < TAMANHO_BLOCO) {  // Deu ruim, não leu 512 bytes, ou seja, não tem
                                // um bloco inteiro pra ler
    fseek(arquivo, -lidos, SEEK_CUR);
    int i;
    for (i = 0; i < TAMANHO_BLOCO; i++) bloco[i] = 0;
    CriarNovoBloco(arquivo);
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
    if (bloco[i] == '*' || bloco[i] == 0) { // * é o caractere pra marcar registros invalidos
      return i;  
    }
  }
  return -1;
}

int InserirRegistro(Arquivo arquivo, Registro r) {
  int resultado = 0;
  int tamanho_total = strlen(r.Nome) + strlen(r.Sobrenome) +
                      1;  // o +1 é por causa do |, que separa os campos
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
  for (; i < TAMANHO_REGISTRO; i++) {
    bloco[posicao + i] = '0';
  }
  resultado = 1;
  SalvarBloco(bloco, arquivo);
  return resultado;
}

int BuscaRegistro(Arquivo arquivo, char chave[TAMANHO_REGISTRO]) {
  rewind(arquivo);
  char leitura;
  int i, j, k;
  char bloco[TAMANHO_BLOCO];
  int achou = 0;
  fread(bloco, sizeof(char), TAMANHO_BLOCO, arquivo); //Lê o bloco já que a função PegaproximoBloco é uma fodida
  while(bloco[0] != EOF) { //Eu imagino que se não tiver mais bloco pra lê ele vai lê EOF
    for(i = 0; i < TAMANHO_BLOCO; i += TAMANHO_REGISTRO) {
      j = i;
      k = 0;
      while(bloco[j] == chave[k] && chave[k] != '\0') { //vai comparar o inicio de cada registro com a chave
        j++;k++;                                        //até o final da chave ou até deixarem de ser iguais
      }
      if(chave[k] == '\0') { //Se chegou no fim da chave, é um match, deve retornar na posição i do arquivo
          achou = 1;
          break; //Esse break deveria sair do for de ler registros
      }
      fread(bloco, sizeof(char), TAMANHO_BLOCO, arquivo); //Como não encontrou até então, pega o próximo bloco
    }
    if (achou == 1) { //se já achou, pode sair do while de ler blocos
      break;
    }
    fread(bloco, sizeof(char), TAMANHO_BLOCO, arquivo); //Como não encontrou até então, pega o próximo bloco
  }
  return achou; //Pra saber se encontrou e está na posição ou não
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
  for (i = 0; i < 10; i++) {
    if (!InserirRegistro(a, registros[i % 6])) {
      printf("Erro ao inserir");
    }
  }
  fclose(a);
  return 0;
}