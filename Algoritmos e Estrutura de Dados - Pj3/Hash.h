#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H

#include <stdio.h>
#include <time.h> //uso e manipula��o do tipo tempo
#include <stdlib.h>
#include <string.h> //uso e manipula��o de strings

//Estrutura para criar uma base de dados de funcion�rios
typedef struct Funcionariooo {
    int id;
    char nome[50];
    char cpf[15];
    char dataNiver[11];
    double salario;
} TFunccc;

//Fun��o para salvar o registro de um funcion�rio
void salvarFuncionario(TFunccc *func, FILE *out);

//Fun��o para criar uma Database de funcion�rios
void criarArquivo(FILE *arq, int nFunc, int *vet);

//Fun��o para ler o registro de um funcion�rio
TFunccc *lerFuncionario(FILE *in);

//Fun��o para imprimir o registro de um funcion�rio
void imprimirFuncionario (TFunccc *func);

//Fun��o para imprimir a database de funcion�rios
void imprimirArquivoFuncionario(FILE *file);

//Fun��o que retorna o tamanho do arquivo
int tamanhoArquivo(FILE *file, int contSizeFile);

//Fun��o que imprime parti��es realizadas
void imprimeParticao(FILE *file, char partitionName[]);

//M�todo da tabela Hash
void hashTable(FILE *file, int numberOfPartition, int sizeFile);

//Fun��o para deletar funcion�rio na tabela hash
void deletar_hash(int employerID, int partitionID);

//Fun��o para inserir funcion�rio na tabela hash
void inserir_hash(TFunccc*funcHashTableInsert,int employerID, int partitionID);

//Fun��o para particionar tabela hash
void particao_hash(int numberOfPartition);

//Fun��o de busca em uma tabela hash
TFunccc *busca_hash(int employerID, int partitionID);

//Fun��o que verifica o vetor de tamanho 6
int percorreVetor(int vet[6]);

//Fun��o para embaralhar com vetor limitado de tamanho 6
void embaralharVetor(int *vet, int vetSize);

//Fun��o que realiza a Sele��o por Substitui��o
int selecaoSubstituicao (FILE *file, char nameFilePartition[]);

//M�todo para Intercalacao Otima
void intercalacaoOtima(int numberOfPartition, char nameFilePartition[]);



#endif
