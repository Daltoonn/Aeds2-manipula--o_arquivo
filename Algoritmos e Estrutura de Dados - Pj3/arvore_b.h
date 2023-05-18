#ifndef ARVORE_B_H_INCLUDED
#define ARVORE_B_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>


typedef struct Funcionarioo
{
    int code;
    char nome[50];
    char data_nascimento[11];
    int salario;
} TFuncc;

void leFuncc(TFuncc funcs);
void createFunc(TFuncc *funcs, int n);
void createArq(TFuncc *funcs, FILE *arq, int n);
void createArqByOne(TFuncc func, FILE *ordenado, int i);
TFuncc readFile(FILE *arq, int i);
void keySorting(FILE *arq, FILE *ordenado, int tamanho);
TFuncc readFileWithByte(FILE *arq);

///////////////////////////
//so da arvore

//estrutura pode ser usada para armazenar um c�digo de identifica��o
typedef struct key_sorting
{
    int code; //cod func
    long RRN; //registro ou indice
} Key_Sorting;

typedef struct Node
{
    //key � oque vai ligar o arquivo( com informa��es dos codigos indice)
    Key_Sorting *key;
    struct Node *left;
    struct Node *right;
} Node;

Node *get_root();
int search(Node *no, int index);
int insert(Node **ppRaiz, Key_Sorting *x, int n, TFuncc *func);
void toString(Node *root);
void remove_tree(Node *root, int id);
void createFuncWithTree(TFuncc *funcs, Node **no, int n);














#endif // ARVORE_B_H_INCLUDED
