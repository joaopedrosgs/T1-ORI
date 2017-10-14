#include <stdio.h>

#include <memory.h>

#define TAMANHO_REGISTRO 100
#define TAMANHO_BLOCO 512
#define TAMANHO_MAXIMO_BLOCO (TAMANHO_BLOCO/TAMANHO_REGISTRO)*TAMANHO_REGISTRO

struct Registro {
    char *Nome;
    char *Sobrenome;
};

struct Arquivo {
    FILE *arquivo;
    int atual;
    int bloco_atual;
};

struct Arquivo CriarArquivo(char *_nome) {
    struct Arquivo a = {fopen(_nome, "w+b"), 0,0};
    if (a.arquivo == NULL) {
        printf("erro");
    }
    return a;
}

int InserirRegistro(struct Arquivo *a, struct Registro r) {
    int tamanho_total = strlen(r.Nome) + strlen(r.Sobrenome) + 1;
    if (tamanho_total > TAMANHO_REGISTRO - 2) {
        return 0;
    }
    if (a->atual % TAMANHO_BLOCO >= TAMANHO_MAXIMO_BLOCO) { // Se não cabe mais nenhum registro inteiro
        a->atual=0;
        a->bloco_atual++; // Vamos para o inicio do proximo bloco
    }
    char buf[tamanho_total+1];
    snprintf(buf, sizeof buf, "%s|%s", r.Nome, r.Sobrenome);
    
    if (a->arquivo != NULL) {
        int lugar =  a->atual+(a->bloco_atual)*TAMANHO_BLOCO;
        fseek(a->arquivo, lugar, SEEK_SET);
        fwrite(buf, tamanho_total,1, a->arquivo);
        fflush(a->arquivo);

    } else {
        printf("Arquivo não aberto");
    }
    a->atual = a->atual+100;
}


void main() {
    struct Arquivo b = CriarArquivo("alaao");
    if(b.arquivo==NULL) {
        printf("erro");
    }
    struct Registro a[] = {{"Joao Pedro", "Sao gregorio Silva"},
                            {"Joao Pedro", "Sao gregorio Silva"},
                            {"Joao Pedro", "Sao gregorio Silva"},
                            {"Joao Pedro", "Sao gregorio Silva"},
                            {"Joaasdasddro", "Saaddorio Silva"},
                            {"Joao Pedro", "Sgorva"},
                            {"Joo", "Sao gregorio Silva"},
                            {"Joao Pedro", "Sao gva"},
                            {"Joao Pesasdasdasdsadasdsaddro", "Sao gregorio Silva"},
                            {"Joao Pedro", "Sao gregoasdasdrio Silva"},
                            {"Joao Pedro", "Sao gregorasdasdadio Silva"},
                            {"Joao Pedro", "Sao gregorio Sasdasdilva"}};
    int i=0;
    int sizeA = sizeof(a)/sizeof(a[0]);
   for(i=0; i<sizeA;i++) {
    InserirRegistro(&b, a[i]);
    
   }
    fclose(b.arquivo);

}