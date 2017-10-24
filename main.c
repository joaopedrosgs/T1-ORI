#include <stdio.h>

#include <memory.h>
#include <stdlib.h>

#define TAMANHO_REGISTRO 100
#define TAMANHO_BLOCO 512
#define CAMPO_NOME 0
#define CAMPO_SOBRENOME 1

typedef struct {
    char *Nome;
    char *Sobrenome;
} Registro;


typedef FILE *Arquivo;

void CriarNovoBloco(Arquivo arquivo) {
    char bloco[TAMANHO_BLOCO] = {0};
    fwrite(bloco, sizeof(char), TAMANHO_BLOCO, arquivo); // Criando bloco novo
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

int PegarProximoBloco(char bloco[TAMANHO_BLOCO], Arquivo arquivo) {
    int lidos = fread(bloco, sizeof(char), TAMANHO_BLOCO, arquivo);
    if (lidos < TAMANHO_BLOCO) { // Deu ruim, não leu 512 bytes, ou seja, não tem
        // um bloco inteiro pra ler
        int i;
        for (i = 0; i < TAMANHO_BLOCO; i++)
            bloco[i] = 0;
    }
    return lidos;
}

void SalvarBloco(char bloco[TAMANHO_BLOCO], Arquivo arquivo) {
    fseek(arquivo, -TAMANHO_BLOCO, SEEK_CUR); // Volta pra salvar em cima
    fwrite(bloco, sizeof(char), TAMANHO_BLOCO, arquivo);
    fflush(arquivo);
}

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

int InserirRegistro(Arquivo arquivo, Registro r) {
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
             j = j + TAMANHO_REGISTRO) {         // Pula de registro em registro
            for (i = 0; i <= tamanho_chave+1; i++) { // enquanto houver chave
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

Registro BuscaRegistro(Arquivo arquivo, char *chave) {
    Registro retorno = {NULL, NULL};
    int local = EncontrarIndice(arquivo, chave, (int) strlen(chave));
    if (local < 0)
        return retorno;
    int numero_bloco = (local / TAMANHO_BLOCO) * TAMANHO_BLOCO;
    fseek(arquivo, numero_bloco, SEEK_SET);
    char bloco[TAMANHO_BLOCO] = {0};
    PegarProximoBloco(bloco, arquivo);
    int numero_registro = (local%TAMANHO_BLOCO) / TAMANHO_REGISTRO;
    int i, preenchendo = CAMPO_NOME;
    int tamanho_nome=0, tamanho_sobrenome=0;
    for (i = 0; i < TAMANHO_REGISTRO; i++) {
        if (bloco[i + (numero_registro*TAMANHO_REGISTRO)] == '|') {
            preenchendo++;
        }else if (preenchendo == CAMPO_NOME) {
            tamanho_nome++;
        } else if (preenchendo == CAMPO_SOBRENOME) {
            if (bloco[i + numero_registro] == 0)
                break;
            tamanho_sobrenome++;
        }
    }
    int posicaoRegistro = numero_registro*TAMANHO_REGISTRO;
    retorno.Nome = malloc(sizeof(char)*tamanho_nome);
    retorno.Sobrenome = malloc(sizeof(char)*tamanho_sobrenome);
    memcpy(retorno.Nome, bloco+posicaoRegistro, sizeof(char)*tamanho_nome);
    memcpy(retorno.Sobrenome, bloco+posicaoRegistro+(sizeof(char)*(tamanho_nome+1)), sizeof(char)*tamanho_sobrenome);
    rewind(arquivo);
    return retorno;
}

int main() {

    Arquivo a = CriarArquivo("dados.txt");
    if (a == NULL) {
        printf("Erro ao abrir o arquivo");
    }
    Registro registros[] = {
            {"Joao Pedro", "Sao gregorio Silva"},
            {"Kaua",       "Ferreira Costa"},
            {"Lara",       "Goncalves Cavalcanti"},
            {"Vitória",    "Gomes Barros"},
            {"Kaua",       "Ferreira Costa"},
            {"baaara",       "Goncalves Cavalcanti"},
            {"dsdteeria",    "Gomes Barros"}};
    int i = 0;
    int sizeA = sizeof(registros) / sizeof(registros[0]);
    for (i = 0; i < 1000; i++) {
        if (!InserirRegistro(a, registros[i % 7])) {
            printf("Erro ao inserir");
        }
    }
    char *chave = "dsdteeria";
    Registro x = BuscaRegistro(a, chave);
    printf("%s", x.Sobrenome);
    fclose(a);
    return 0;
}