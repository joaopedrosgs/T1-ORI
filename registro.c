#include "registro.h"
#include "bloco.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
// PosicaoParaInsercao procura uma posição valida para inserir informação em um
// bloco
int PosicaoParaInsercao(char bloco[TAMANHO_BLOCO]) {
  int i = 0;
  for (i = 0; i + TAMANHO_REGISTRO < TAMANHO_BLOCO; i += TAMANHO_REGISTRO) {
    // * é o caractere pra marcar registros invalidos
    if (bloco[i] == '*' || bloco[i] == 0) {
      return i;
    }
  }
  return -1;
}
// CodificarRegistro transforma uma struct Registro em uma string pronta para
// ser inserida no bloco
char *CodificarRegistro(Registro r) {
  // o 2 é por causa do | e do \0, que separa os campos do registro
  int tamanho_total = strlen(r.Nome) + strlen(r.Sobrenome) + 2;
  // Retorne falso se é muito grande ou vazio
  if (tamanho_total > TAMANHO_REGISTRO || tamanho_total <= 2) {
    return NULL;
  }
  char *buffer = (char *)calloc(tamanho_total, sizeof(char));
  snprintf(buffer, tamanho_total, "%s|%s", r.Nome, r.Sobrenome);
  return buffer;
}

int InserirRegistro(FILE *arquivo, Registro r) {
  int resultado = 0;
  char *buffer = CodificarRegistro(r);
  if (buffer != NULL) {
    rewind(arquivo);
    char bloco[TAMANHO_BLOCO] = {0};
    int posicao = -1;
    while (posicao == -1) {
      // Enquanto não achou uma posicao para inserir no bloco
      int lidos = PegarProximoBloco(bloco, arquivo);
      if (lidos < TAMANHO_BLOCO) {
        // Volta caso tenha lido algo incorreto
        fseek(arquivo, -lidos, SEEK_CUR);
        CriarNovoBloco(arquivo);
      }
      posicao = PosicaoParaInsercao(bloco);
    };

    memcpy(bloco + posicao, buffer, strlen(buffer) * sizeof(char));
    free(buffer);
    resultado = 1;
    SalvarBloco(bloco, arquivo);
    rewind(arquivo);
  }
  return resultado;
}

// EncontrarIndice encontra o primeiro espaço vazio para inserir em um bloco
int EncontrarIndice(Arquivo arquivo, char *chave, int tamanho_chave) {
  rewind(arquivo);
  // Não é possivel encontrar um registro deletado
  if (chave[0] == '*')
    return -1;
  char bloco[TAMANHO_BLOCO] = {0};
  int i = 0, j = 0, lidos, numero_bloco = -1;
  do {
    lidos = PegarProximoBloco(bloco, arquivo);
    numero_bloco++;
    // Pula de registro em registro
    for (int j = 0; j + TAMANHO_REGISTRO <= TAMANHO_BLOCO;
         j = j + TAMANHO_REGISTRO) {
      // enquanto houver chave
      for (i = 0; i <= tamanho_chave + 1; i++) {
        if (chave[i] == 0) {
          // Achou
          return j + (numero_bloco * TAMANHO_BLOCO);
        }
        // Não é o registro procurado
        if (bloco[i + j] != chave[i]) {
          break;
        }
      }
    }
  } while (lidos == TAMANHO_BLOCO);
  return -1;
}
// DecodificarRegistro transforma uma string lida do bloco em uma struct
// Registro (funciona como uma maquina de estados, onde em cada estado
// preenche-se um campo da struct Registro)
Registro DecodificarRegistro(char *texto) {
  Registro registro;
  int i, preenchendo = CAMPO_NOME;
  int tamanho_nome = 0, tamanho_sobrenome = 0;
  for (i = 0; i < TAMANHO_REGISTRO; i++) {
    if (texto[i] == '|') {
      preenchendo++;
    } else if (preenchendo == CAMPO_NOME) {
      tamanho_nome++;
    } else if (preenchendo == CAMPO_SOBRENOME) {
      if (texto[i] == 0)
        break;
      tamanho_sobrenome++;
    }
  }
  registro.Nome = (char *)calloc(
      tamanho_nome + 1,
      sizeof(char)); // é uma boa ideia alocar dinamicamente isso?
  registro.Sobrenome = (char *)calloc(tamanho_sobrenome + 1, sizeof(char));
  memcpy(registro.Nome, texto, sizeof(char) * tamanho_nome);
  memcpy(registro.Sobrenome, texto + (sizeof(char) * (tamanho_nome + 1)),
         sizeof(char) * tamanho_sobrenome);
  return registro;
}

Registro BuscaRegistro(Arquivo arquivo, char *chave) {
  Registro retorno = {NULL, NULL};

  int local = EncontrarIndice(arquivo, chave, (int)strlen(chave));
  if (local < 0)
    return retorno;
  int numero_bloco = (local / TAMANHO_BLOCO) * TAMANHO_BLOCO;
  fseek(arquivo, numero_bloco, SEEK_SET);
  char bloco[TAMANHO_BLOCO] = {0};
  PegarProximoBloco(bloco, arquivo);
  int numero_registro = (local % TAMANHO_BLOCO) / TAMANHO_REGISTRO;
  char texto[TAMANHO_REGISTRO];
  memcpy(texto, bloco + (numero_registro * TAMANHO_REGISTRO), TAMANHO_REGISTRO);
  retorno = DecodificarRegistro(texto);
  return retorno;
}
// Marca o registro com o caractere de deletado (*)
Registro RemoverRegistro(Arquivo arquivo, char *chave) {
  rewind(arquivo);
  Registro registro;
  int local = EncontrarIndice(arquivo, chave, (int)strlen(chave));
  if (local >= 0) {
    int numero_bloco = (local / TAMANHO_BLOCO) * TAMANHO_BLOCO;
    fseek(arquivo, numero_bloco, SEEK_SET);
    char bloco[TAMANHO_BLOCO] = {0};
    PegarProximoBloco(bloco, arquivo);
    int local_no_bloco = local % TAMANHO_BLOCO;
    registro = DecodificarRegistro(bloco + local_no_bloco);
    bloco[local_no_bloco] = '*';
    SalvarBloco(bloco, arquivo);
  }
  return registro;
}

void RemoverRegistroDaMemoria(Registro r) {
  free(r.Nome);
  free(r.Sobrenome);
}
// Permite listar registros (inclusive os deletados)
void ListarRegistros(Arquivo arquivo, int imprimir_deletados) {
  rewind(arquivo);
  char bloco[TAMANHO_BLOCO] = {0};
  int n_registro, n_bloco = 0;
  while (PegarProximoBloco(bloco, arquivo) == TAMANHO_BLOCO) {
    printf("%d : {", n_bloco);
    for (n_registro = 0; n_registro + TAMANHO_REGISTRO < TAMANHO_BLOCO;
         n_registro += TAMANHO_REGISTRO) {
      if ((*(bloco + n_registro) == '*' && !imprimir_deletados) ||
          *(bloco + n_registro) == 0)
        continue;
      Registro atual = DecodificarRegistro(bloco + n_registro);
      printf("\t{id=%-5d,\tnome='%-10s',\tsobrenome='%-25s'},\n",
             n_registro + (n_bloco * TAMANHO_BLOCO), atual.Nome,
             atual.Sobrenome);
      RemoverRegistroDaMemoria(atual);
    }
    n_bloco++;
    printf("},\n");
  }
}