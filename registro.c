//
// Created by pedro on 24/10/17.
//

#include <stdio.h>
#include "registro.h"

typedef struct {
    char *Nome;
    char *Sobrenome;
} Registro;

int PosicaoParaInsercao(char bloco[TAMANHO_BLOCO]) {
    int i = 0;
    for (i = 0; i + TAMANHO_REGISTRO < TAMANHO_BLOCO; i += TAMANHO_REGISTRO) {
        if (bloco[i] == '*' ||
            bloco[i] == 0) { // * é o caractere pra marcar registros invalidos
            return i;
        }
    }
    return -1;
}

int InserirRegistro(FILE *arquivo, Registro r) {
    int resultado = 0;
    int tamanho_total = strlen(r.Nome) + strlen(r.Sobrenome) +
                        1; // o +1 é por causa do |, que separa os campos
    if (tamanho_total > TAMANHO_REGISTRO) {
        return resultado;
    }

    rewind(arquivo);

    char bloco[TAMANHO_BLOCO] = {0};
    int posicao = -1;
    while (posicao < 0) {
        int lidos = PegarProximoBloco(bloco, arquivo);
        if (lidos < TAMANHO_BLOCO) {
            fseek(arquivo, -lidos, SEEK_CUR);
            CriarNovoBloco(arquivo);
        }
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
    rewind(arquivo);
    return resultado;
}

int EncontrarIndice(Arquivo arquivo, char *chave, int tamanho_chave) {
    rewind(arquivo);
    char bloco[TAMANHO_BLOCO] = {0};
    int i = 0, j = 0, lidos, numero_bloco = -1;
    do {
        lidos = PegarProximoBloco(bloco, arquivo);
        numero_bloco++;
        for (int j = 0; j + TAMANHO_REGISTRO <= TAMANHO_BLOCO;
             j = j + TAMANHO_REGISTRO) {             // Pula de registro em registro
            for (i = 0; i <= tamanho_chave + 1; i++) { // enquanto houver chave
                if (chave[i] == 0) {
                    return j + (numero_bloco * TAMANHO_BLOCO); // Achou
                }
                if (bloco[i + j] != chave[i]) { // Não é o registro procurado
                    break;
                }
            }
        }
    } while (lidos == TAMANHO_BLOCO);
    return -1;
}

Registro DecodificarRegistro(char texto[TAMANHO_REGISTRO]) {
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
    registro.Nome = malloc(sizeof(char) * tamanho_nome);
    registro.Sobrenome = malloc(sizeof(char) * tamanho_sobrenome);
    memcpy(registro.Nome, texto, sizeof(char) * tamanho_nome);
    memcpy(registro.Sobrenome, texto + (sizeof(char) * (tamanho_nome + 1)),
           sizeof(char) * tamanho_sobrenome);
    return registro;
}

Registro BuscaRegistro(Arquivo arquivo, char *chave) {
    Registro retorno = {NULL, NULL};
    int local = EncontrarIndice(arquivo, chave, (int) strlen(chave));
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

Registro RemoverRegistro(Arquivo arquivo, char *chave) {
    rewind(arquivo);
    Registro registro;
    int local = EncontrarIndice(arquivo, chave, (int) strlen(chave));
    if (local > 0) {
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