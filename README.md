
# T1-ORI

#Descrição das funções do programa:


**CriarArquivo**
- Retorno: Ponteiro para FILE.
- Argumentos: nome = vetor de char.
- Processo: Tenta criar um arquivo com o nome contido na variável nome ou sobrescreve caso já exista. Se houver algum problema ao abrir, imprime "Problema ao abrir o arquivo". Se não houve nenhum problema em abrir/criar o arquivo, executa a função CriarNovoBloco. Após isso, retorna o arquivo criado.

**AbrirArquivo**
- Retorno: Ponteiro para FILE.
- Argumentos: nome = vetor de char.
- Processo: Procura e abre um arquivo com o nome contido na variável nome, caso haja algum problema imprime "Problema ao abrir o arquivo". Após isso, retorna o arquivo aberto.

**CompactarArquivo**
- Retorno: Nenhum.
- Argumentos: arquivo = ponteiro para FILE; nome_arquivo = vetor de char.
- Processo: Cria uma nova variável ponteiro para FILE com o nome de novo, lê todos os registros válidos dos blocos de arquivo e os escreve em novo. Ao final, arquivo torna-se uma cópia de novo e seu antigo arquivo, com o nome contido em nome_arquivo, é apagado.

**FecharArquivo**
- Retorno: Nenhum.
- Argumentos: arquivo = ponteiro para FILE.
- Processo: Executa a função CompactarArquivo em arquivo e depois o fecha com fclose.

**CriarNovoBloco**
- Retorno: Nenhum.
- Argumentos: arquivo = ponteiro para FILE.
- Processo: Cria um vetor de caracteres chamado bloco de tamanho TAMANHO_BLOCO e o escreve na última posição do ponteiro de escrita em arquivo.

**PegarProximoBloco**
- Retorno: Inteiro.
- Argumentos: bloco = vetor de char do tamanho TAMANHO_BLOCO; arquivo =  ponteiro para FILE.
- Processo: Tenta ler TAMANHO_BLOCO de caracteres do arquivo, colocando-o no vetor de char que foi passado por referencia, caso contrario, zera este mesmo vetor e retorna o numero de elementos lidos

**SalvarBloco**
- Retorno: Nenhum.
- Argumentos: bloco = vetor de char do tamanho TAMANHO_BLOCO, arquivo = ponteiro para FILE.
- Processo: Sobrescreve o bloco na posição em que ele se encontrava no arquivo ou no final do arquivo caso um novo bloco tenha sido criado.

**PosicaoParaInsercao**
- Retorno: Inteiro.
- Argumentos: bloco = vetor de char de tamanho TAMANHO_BLOCO.
- Processo: Percorre o vetor bloco até encontrar a primeira ocorrência de * ou 0, que são indicativos de espaço disponível para escrita. Retorna a posição dessa ocorrência no vetor ou -1 caso não haja nenhuma.

**InserirRegistro**
- Retorno: Inteiro.
- Argumentos: arquivo = ponteiro para FILE, r = struct do tipo Registro.
- Processo: Retorna 0 caso o registro r possua um comprimento inválido. Pega o próximo bloco do arquivo e encontra nele, através da função PosicaoParaInsercao o próximo espaço disponível para inserir o registro r. Caso não haja espaço nesse bloco, um novo bloco é criado. Escreve então o registro na posição encontrada no bloco encontrado e retorna 1 para indicar sucesso da operação.

**EncontrarIndice**
- Retorno: Inteiro.
- Argumentos: arquivo = ponteiro para FILE; chave = vetor de char; tamanho_chave = inteiro. 
- Processo: Percorre cada bloco do arquivo, comparando cada registro com chave até encontrar uma correspondência ou até acabarem os blocos do arquivo. Retorna a posição da correspondência ou -1 caso não haja.

**DecodificarRegistro**
- Retorno: Struct do tipo Registro.
- Argumentos: texto = vetor de char.
- Processo: Recebe em texto uma sequência de caracteres que é remodelada de forma a ser escrito no formato de uma struct do tipo Registro, que é então retornada.

**BuscaRegistro**
- Retorno: struct do tipo Registro.
- Argumentos: arquivo = ponteiro para FILE; chave = vetor de char.
- Processo: Busca o índice através de EncontrarIndice e converte com DecodificarRegistro o registro na posição retornada por EncontrarIndice com a chave contida em chave. Após isso, retorna o Registro convertido ou 0 caso tenha falhado em encontrar ou converter.


**RemoverRegistro**
- Retorno: struct do tipo Registro.
- Argumentos: arquivo = ponteiro para FILE; chave = vetor de char.
- Processo: Busca o índice através de EncontrarIndice com a chave contida em chave. Na posição retornada, escreve o caracter *, que invalida o registro em questão. Salva o bloco e retorna o registro como ficou após a alteração com DecodificarRegistro.

**RemoverRegistroDaMemoria**
- Retorno: Nenhum. 
- Argumentos: r = struct do tipo Registro.
- Processo: Desaloca os campos de r da memória.

**ListarRegistros**
- Retorno: Nenhum.
- Argumentos: arquivo = ponteiro para FILE; imprimir_deletados = inteiro.
- Processo: Percorre cada um dos blocos de arquivo com PegarProximoBloco, imprimindo cada um dos registros válidos após passá-los pela função DecodificarRegistro. Se imprimir_deletados valer 1, imprime também os registros marcados como inválidos.


*#Descrição das constantes do programa:*

**Registro**
- struct com campos:
- Nome = vetor de char;
- Sobrenome = vetor de char;
- Armazena as informações de um registro.

**TAMANHO_REGISTRO**:
Determina o tamanho fixo de cada registro.

**TAMANHO_BLOCO**:
Determina o tamanho fixo de cada bloco.

**CAMPO_NOME**:
Determina a posição do campo que possui o valor de nome no registro.

**CAMPO_SOBRENOME**:
Determina a posição do campo que possui o valor de sobrenome no registro.


