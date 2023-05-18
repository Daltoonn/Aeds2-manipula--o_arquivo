#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"



//Criando base de dados dos funcion�rios
void criarArquivo(FILE *arq, int nFunc, int *vet) {
    for(int i = 1; i <= nFunc; i++) {

        printf("Criando funcionario %i..\n", i);
        TFunccc func;

        func.id = vet[i - 1];

        char auxName[15];
        for(int j = 0; j < 15; j++) {
            auxName[j]= 'a' + (char)(rand()%26); //cria um nome de 15 caracteres aleatoriamente com as 26 letras do alfabeto
        }

        //a fun��o sprintf formata o tipo char e armazena em um vetor
        sprintf(func.nome, "%s", auxName);
        sprintf(func.cpf, "XXX.XXX.XX-XX");
        sprintf(func.dataNiver, "01/08/2021");
        func.salario = 1500 + rand() % 15000;
        fseek(arq, (i-1)* sizeof(TFunccc), SEEK_SET); //posicionar o cursor do arquivo na posi��o correta
        salvarFuncionario(&func, arq);

        //Para imprimir na tela funcion�rio por funcion�rio
        printf("Dados do funcionario %i:\n", i);
        printf("ID: %d\n", func.id);
        printf("Nome: %s\n", func.nome);
        printf("CPF: %s\n", func.cpf);
        printf("Data de Nascimento: %s\n", func.dataNiver);
        printf("Salario: R$%.2f\n", func.salario);
        printf("--------------------\n");
    }
}



//objeto da struct TFunc recebe ponteiro em um arquivo no modo de escrita
//Fun��o para ler o registro de um funcion�rio
TFunccc *lerFuncionario(FILE *in) {
    TFunccc *func = (TFunccc*) malloc (sizeof(TFunccc)); //aloca��o de mem�ria
    if ( 0 >= fread(&func->id, sizeof(int), 1, in)) { //enquanto a leitura for sendo realizada em arquivo vazio, libera o funcion�rio da mem�ria e retorna NULL
        free(func);
        return NULL;
    }

    //Caso contr�rio, ele l� todos os elementos da struct
    fread(func->nome, sizeof(char), sizeof(func->nome), in);
    fread(func->cpf, sizeof(char), sizeof(func->cpf), in);
    fread(func->dataNiver, sizeof(char), sizeof(func->dataNiver), in);
    fread(&func->salario, sizeof(double), 1, in);
    return func; // o ponteiro retorna para o registro preenchido
}


//Fun��o para salvar registro de funcion�rio
//ponteiro da estrutura e salva no arquivo
//fwrite � usada para escrever um bloco de dados em um arquivo
void salvarFuncionario(TFunccc *func, FILE *out) {
    fwrite(&func->id, sizeof(int), 1, out);
    fwrite(func->nome, sizeof(char), sizeof(func->nome), out);
    fwrite(func->cpf, sizeof(char), sizeof(func->cpf), out);
    fwrite(func->dataNiver, sizeof(char), sizeof(func->dataNiver), out);
    fwrite(&func->salario, sizeof(double), 1, out);
}


//Fun��o para imprimir funcion�rio
void imprimirFuncionario(TFunccc *func) {
    if (func == NULL) { //a mensagem de erro � exibida apenas se o ponteiro para o funcion�rio for nulo, e a impress�o das informa��es do funcion�rio s� ocorre se o ponteiro for v�lido
        printf("Funcionario nao encontrado.\n");
    } else {
        printf("Identificador: %d\n", func->id);
        printf("Nome: %s\n", func->nome);
        printf("CPF: %s\n", func->cpf);
        printf("Data de Aniversario: %s\n", func->dataNiver);
        printf("Salario: %.2f\n", func->salario);
        printf("----------------------------------------------- \n");
    }
}




//Fun��o que retorna o tamanho do arquivo
int tamanhoArquivo(FILE *file, int contSizeFile) {
    int bytesAUX = 0;
    while(!feof(file)) { //retorna um valor diferente de zero se uma opera��o de leitura tentou ler ap�s o final do arquivo
        fseek(file, bytesAUX * sizeof(TFunccc), SEEK_SET); //posicionar o ponteiro do arquivo na posi��o do pr�ximo registro "TFunc" a ser lido
        TFunccc *aux = lerFuncionario(file); //armazenando a leitura do registro em uma vari�vel auxiliar
        if(aux != NULL) {
            contSizeFile++;
        }
        bytesAUX++;
    }
    return contSizeFile;
}




//Fun��o para embaralhar
void embaralharVetor(int *vet, int vetSize)
{

    for (int i = 0; i < vetSize; i++) {
        vet[i] = i + 1;
    }

    for (int i = 0; i < vetSize; i++) //a cada itera��o, seleciona aleatoriamente um �ndice do vetor e troca o elemento atual com o elemento na posi��o aleat�ria
    {
        int r = rand() % vetSize; //gerar n�meros aleat�rios de acordo com o tamanho do vetor

        int temp = vet[i];
        vet[i] = vet[r];
        vet[r] = temp;
    }

}




//FUNCOESSSS HASHHH



//Fun��o para particionar tabela hash
//cria um n�mero espec�fico de parti��es onde cada parti��o � armazenada em um arquivo bin�rio com um nome exclusivo
void particao_hash(int numberOfPartition) {

    for (int i = 0; i < numberOfPartition; ++i) {

        char partitionName[100];
        char str1[100];
        char str2[100] = ".dat";

        //converte um valor inteiro em uma string.
        itoa(i, str1, 10);
        // "strcat" concatena duas strings em um �nico buffer
        strcat(strcpy(partitionName, "hashTablePartition"), str1);
        strcat(strcpy(partitionName, partitionName), str2);

        FILE *file = fopen(partitionName,"wb+");

        fclose(file);
    }
}







//M�todo da tabela Hash, implementa a cria��o de uma tabela hash, size file funcionarios
void hashTable(FILE *file, int numberOfPartition, int sizeFile) {

    //colocando no inicio
    rewind(file);

    char partitionName[100];
    char str1[100];
    char str2[100] = ".dat";

    for (int i = 0; i < sizeFile; ++i) { //leitura de cada registro do arquivo original

        TFunccc *auxFunc = lerFuncionario(file);

        int selectedParticipation = auxFunc->id % numberOfPartition; //fun��o de resto de divis�o

        //converte selectedParticipation em uma string de caracteres usando a fun��o itoa()
        // a concatena com outras strings para formar o nome do arquivo de parti��
        itoa(selectedParticipation, str1, 10);
        strcat(strcpy(partitionName, "hashTablePartition"), str1);
        strcat(strcpy(partitionName, partitionName), str2);

        FILE *filePartition = fopen(partitionName,"ab+");

        salvarFuncionario(auxFunc, filePartition); //registro esta sendo salvo no arquivo de parti��o criado

        fclose(filePartition);

    }


    //passa por cada parti��o criada e imprime os registros armazenados nessa parti��o
    for (int i = 0; i < numberOfPartition; ++i) { //imprimir os registros de cada parti��o criada

        itoa(i, str1, 10);
        strcat(strcpy(partitionName, "hashTablePartition"), str1);
        strcat(strcpy(partitionName, partitionName), str2);

        FILE *filePartition = fopen(partitionName,"rb+");

        imprimeParticao(filePartition, partitionName); //imprime os registros no arquivo de parti��o criado

        fclose(filePartition);
    }
}







//M�todo de ordena��o Externa (Intercala��o �tima)
void imprimeParticao(FILE *file, char partitionName[]) {
    printf("\nos IDs apos a particao de funcionarios no arquivo  %s: \n --->  ", partitionName);
    for (int i = 0; i < tamanhoArquivo(file, 0); ++i) {
        fseek(file, i * sizeof(TFunccc), SEEK_SET) ; //garantir que as parti��es estejam corretas
        TFunccc *aux = lerFuncionario(file); //leitura do registro do funcion�rio
        printf(" %i ", aux->id);
    }
    printf("\n");
}






//Fun��o para inserir funcion�rio na tabela hash
//parametros ponteiro estrutura TFunccc, identificador funcionario, indentificador da parti��o
void inserir_hash(TFunccc *funcHashTableInsert,int employerID, int partitionID) {

    char partitionName[100];
    char str1[100];
    char str2[100] = ".dat";
    int totalCompararisons = 0;
    int totalCompararisonsDeleted = 0;
    int sizefilePartition = 0;

    //converte o n�mero de parti��o em uma string para ficar legal de imprimir, junta duas string a string "hashTablePartition"
    itoa(partitionID, str1, 10);
    strcat(strcpy(partitionName, "hashTablePartition"), str1);
    strcat(strcpy(partitionName, partitionName), str2);

    FILE *filePartition = fopen(partitionName,"r+b"); //abertura do arquivo para leitura e grava��o

    //fun��o "tamanhoArquivo" para obter o tamanho do arquivo e define o ponteiro do arquivo para o in�cio do arquivo.
    sizefilePartition = tamanhoArquivo(filePartition, 0);
    rewind(filePartition);


    //evitar colisoess

    TFunccc *auxFunc = buscaSequencial(funcHashTableInsert->id, filePartition, &totalCompararisons); //busca sequencial para localizar o identificador na tabela hash
    rewind(filePartition);

    TFunccc *auxFuncDeleted = buscaSequencial(999999999, filePartition, &totalCompararisonsDeleted); //busca sequencial para verificar se existe um espa�o vazio
    rewind(filePartition);

    if (auxFunc == NULL && auxFuncDeleted == NULL) {
            //"fseek" para definir o ponteiro do arquivo para a posi��o do registro.
            fseek(filePartition, (sizefilePartition) * sizeof(TFunccc), SEEK_SET);\
            salvarFuncionario(funcHashTableInsert, filePartition);
            printf("\nFuncionario inserido com sucesso no final");

    } else if (auxFunc == NULL && auxFuncDeleted != NULL){
            //"fseek" para definir o ponteiro do arquivo para a posi��o do registro.
            fseek(filePartition, (totalCompararisonsDeleted - 1) * sizeof(TFunccc), SEEK_SET);\
            salvarFuncionario(funcHashTableInsert, filePartition);
            printf("\nFuncionario inserido com sucesso no funcionario excluido anteriormente");

    } else {
        printf("\nFuncionario ja existe na tabela Hash");
    }

    fclose(filePartition);
}






//Fun��o para deletar funcion�rio na tabela hash
//parametro � o codigo do funcionario e o indice da parti��o
void deletar_hash(int employerID, int partitionID) {

    char partitionName[100];
    char str1[100];
    char str2[100] = ".dat";
    int totalCompararisons = 0;

    //converte o n�mero de parti��o em uma string, concatena a string
    itoa(partitionID, str1, 10);
    strcat(strcpy(partitionName, "hashTablePartition"), str1);
    strcat(strcpy(partitionName, partitionName), str2);

    FILE *filePartition = fopen(partitionName,"r+b"); //abertura do arquivo para leitura e grava��o

    TFunccc *auxFunc = buscaSequencial(employerID, filePartition, &totalCompararisons); //uso da busca sequencial para localizar o funcionario em arquivo particionado criado

    if (auxFunc != NULL) {

        //"fseek" para definir o ponteiro do arquivo para a posi��o do registro a ser exclu�do.
        fseek(filePartition, (totalCompararisons - 1) * sizeof(TFunccc), SEEK_SET); //calculo da posi��o

        TFunccc funcHashDelet;

        //cria um registro fict�cio para manter a integridade da tabela hash e garantir que a lista encadeada n�o seja interrompida
        funcHashDelet.id = 999999999;
        sprintf(funcHashDelet.nome, "%s", auxFunc->nome);
        sprintf(funcHashDelet.cpf, "%s", auxFunc->cpf);
        sprintf(funcHashDelet.dataNiver, "%s", auxFunc->dataNiver);
        funcHashDelet.salario = auxFunc->salario;

        //registro fict�cio � salvo
        salvarFuncionario(&funcHashDelet, filePartition);

        printf("\nFuncionario excluido com sucesso");
    } else {
        printf("\nFuncionario nao foi encontrado na tabela Hash");
    }

    fclose(filePartition);
}






//Fun��o de busca em uma tabela hash
TFunccc *busca_hash(int employerID, int partitionID) {

    char partitionName[100];
    char str1[100];
    char str2[100] = ".dat";
    int totalCompararisons;

    itoa(partitionID, str1, 10);
    strcat(strcpy(partitionName, "hashTablePartition"), str1);
    strcat(strcpy(partitionName, partitionName), str2);

    printf("\nProcurando pelo funcionario %s...", partitionName);

    FILE *filePartition = fopen(partitionName,"rb+"); //abertura do arquivo bin�rio para leitura

    TFunccc *auxFunc = buscaSequencial(employerID, filePartition, &totalCompararisons); //realiza��o da busca sequencial no arquivo particionado espec�fico

    fclose(filePartition);

    return auxFunc; //retorna o funcionario de acordo com o identificador
}
