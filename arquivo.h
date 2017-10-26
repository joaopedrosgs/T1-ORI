//
// Created by pedro on 24/10/17.
//

#ifndef T2_ORI_ARQUIVO_H
#define T2_ORI_ARQUIVO_H
typedef FILE *Arquivo;
FILE *CriarArquivo(char *nome);
FILE *AbrirArquivo(char *nome);
void CompactarArquivo(Arquivo arquivo, char* nome_arquivo);
void FecharArquivo(Arquivo arquivo);
#endif //UNTITLED2_ARQUIVO_H
