#include "arvore_b.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

//OPERA�OESSSS COM A ARVOREEEEEEEEEEEEE B

//le funcionario
void leFuncc(TFuncc funcs)
{
    printf("Nome: %s\n", funcs.nome);
    printf("Nascimento: %s\n", funcs.data_nascimento);
    printf("Codigo: %d\n", funcs.code);
    printf("Salario:R$ %d\n", funcs.salario);
}

//cria funcionario
void createFunc(TFuncc *funcs, int n)
{
    int cont = 0;
    int Funcionarios_Desordenados[1000];
    while(cont < 1000){
            //rand � uma fun��o da biblioteca do C, que gera valores aleatorios
        int num = rand();
        int aux = 0;
        for(int i = 0; i < cont; i++){
            if(Funcionarios_Desordenados[i] == num){        // Compara todos os elementos do vetor para ver se ser� numero aleatorio
                aux = 1;  //Se for igual recebe 1
                break;
            }
        }
        if(!aux){                                           // Aux sendo igaul a 1, ele n�o entrar� no while
            Funcionarios_Desordenados[cont] = num;
            cont++;
        }
    }

    for (int j = 0; j < n; j++)
    {
        funcs[j].code = Funcionarios_Desordenados[j];
        sprintf(funcs[j].nome, "Funcionario N%d", Funcionarios_Desordenados[j]);
        funcs[j].salario = rand();
        strcpy(funcs[j].data_nascimento, "XX/XX/XXXX");
    }
}

//cria o arquivo
void createArq(TFuncc *funcs, FILE *arq, int n)
{
    if (arq != NULL)
    {
        for (int i = 0; i < n; i++)
        {
            fseek(arq, (i) * sizeof(TFuncc), SEEK_SET);
            fwrite(&funcs[i], sizeof(TFunc), 1, arq);
        }
    }

    fclose(arq);
}

//fun��o da key cria arquivo binario, esta fun��o grava um valor da estrutura TFuncc em um arquivo na posi��o especificada por i
void createArqByOne(TFuncc func, FILE *ordenado, int i)
{
    //verifica se ele n�o � nulo
    if (ordenado != NULL)
    {
        //vai para o inicio, e depois calcula a posicao correta
        fseek(ordenado, (i) * sizeof(TFuncc), SEEK_SET);
        //escreve no arquivo
        fwrite(&func, sizeof(TFuncc), 1, ordenado);
    }
}


//fun��o � respons�vel por ler um registro do tipo TFuncc a partir de um arquivo bin�rio, a partir de uma posi��o espec�fica no arquivo
//Search CHAMA esta fun��o e ela l� um valor da estrutura TFuncc a partir de um arquivo na posi��o especificada por i. que no caso � a fun��o search de busca
TFuncc readFile(FILE *arq, int i)
{
    TFuncc func;

    //fseek posiciona
    fseek(arq, i * sizeof(TFuncc), SEEK_SET);
    fread(&func, sizeof(TFuncc), 1, arq);

    return func;
}

//le partir de um arquivo, lendo apenas o tamanho do valor
TFuncc readFileWithByte(FILE *arq)
{
    TFuncc func;
    //"fread" para ler o que est� escrito naquele peda�o do arquivo
    fread(&func, sizeof(TFunc), 1, arq);

    return func;
}


//classifica��o chave
//� usado para organizar uma lista de coisas em ordem crescente com base no c�digo
//fica comparando uma com outra para ORDENARRRR
void keySorting(FILE *arq, FILE *ordenado, int tamanho)
{
    double time_spent = 0.0;

    clock_t begin = clock();

    Key_Sorting array[tamanho];

//posiciona para o fim
    rewind(arq);

//percorre o arquivo bin�rio e preenche o array com os c�digos e a posi��o de cada registro no arquivo
    for (int i = 0; i < tamanho; i++)
    {
        fseek(arq, i * sizeof(TFuncc), SEEK_SET);
        array[i].RRN = ftell(arq);
        TFuncc func = readFile(arq, i);
        array[i].code = func.code;
    }

//c�digo ordena o array de acordo com os c�digos das fun��es , compara qual � maior que qual
    for (int i = 0; i < tamanho; i++)
    {
        for (int j = i + 1; j < tamanho; j++)
        {
            if (array[i].code > array[j].code)
            {
                Key_Sorting aux = array[i];
                array[i] = array[j];
                array[j] = aux;
            }
        }
    }

    rewind(arq);
//o, usando a posi��o de cada registro no array ordenado para ler os registros do arquivo em ordem e escrev�-los em um novo arquivo ordenado usando a fun��o "createArqByOne()".
    for (int i = 0; i < tamanho; i++)
    {
        fseek(arq, array[i].RRN, SEEK_SET);
        TFuncc func = readFileWithByte(arq);

        createArqByOne(func, ordenado, i);
    }

    clock_t end = clock();

    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

    printf("\nTempo de processamento do key sorting: %.2f\n\n", time_spent);

    fclose(arq);
}



////////////////
///////////////////////
//FUNCOESSSSSSSSSSSSSSS DA ARVOREEEEEEE


//cria um novo n� para uma �rvore bin�ria de busca. A fun��o recebe como par�metro um ponteiro para um objeto Key_Sorting
//preenchendo-o com uma c�pia da chave passada como par�metro e alocando a mem�ria necess�ria para o novo n� e sua chave.
//peguei do codigo e do slide da tatiana
Node *criaNo(Key_Sorting *x)
{
    // fun��o aloca mem�ria para um novo n� e para uma nova chave dentro do n�
    Node *novo_no = (Node *)malloc(sizeof(Node));
    novo_no->key = (Key_Sorting *)malloc(sizeof(Key_Sorting));
    novo_no->key->code = x->code;
    novo_no->key->RRN = x->RRN;
    novo_no->left = NULL;
    novo_no->right = NULL;
    return (novo_no);
};

//particiona se nao de certo
//insere um n� em uma �rvore bin�ria de busca, representada pelo ponteiro ppRaiz
int insert(Node **ppRaiz, Key_Sorting *x, int n, TFuncc *func)
{

    //a fun��o cria um arquivo bin�rio com as informa��es contidas em func
    if (*ppRaiz == NULL)
    {
        FILE *arq_file = fopen("binario_arvore_b.dat", "rb+");

        createArqByOne(*func, arq_file, n);

        fclose(arq_file);

        //cria um novo n� usando a fun��o criaNo() e atribui o ponteiro desse n� para ppRaiz.
        *ppRaiz = criaNo(x);

        return 1;
    }

   //particiona
   //Se a �rvore n�o estiver vazia, fun��o verifica se o c�digo contido no n� a ser inserido � menor que o c�digo contido no n� atual
   //Se for maior vai para esquerda Left , se for maior Direita
    if (x->code < (*ppRaiz)->key->code)
        return insert(&((*ppRaiz)->left), x, n, func);
    if (x->code > (*ppRaiz)->key->code)
        return insert(&((*ppRaiz)->right), x, n, func);
    return 0;
};


// � uma fun��o que cria uma �rvore bin�ria de busca a partir de um conjunto de dados contido em um vetor de estruturas do tipo TFuncc.
// o segundo parametro que esqueso que fez eu ter raiva demaisss significa um ponteiro para um ponteiro para um n� da �rvore o segundo
void createFuncWithTree(TFuncc *funcs, Node **no, int n)
{

    //ciar um ponteiro para uma estrutura
    Key_Sorting *key = (Key_Sorting *)malloc(sizeof(Key_Sorting));

    //fun��o percorre todo o vetor de
    for (int j = 0; j < n; j++)
    {
        key->code = funcs[j].code;
        key->RRN = j;
        //verificar se o n� raiz � nulo, caso seja, ela cria um novo n� com a chave e o RRN passados como par�metros e insere os dados em um arquivo bin�rio utilizando a fun��o createArqByOne
        insert(no, key, j, &funcs[j]);
    }
}

//busca mais facil
//recursividadeeee
int search(Node *no, int code)
{
    if (no == NULL)
    {
        printf("Arvore nula!");

        return 0;
    }
    //se for menor vai para esquerda
    else if (code < no->key->code)
    {
        return search(no->left, code);
    }
    //se for maior vai para a direita
    else if (code > no->key->code)
    {
        return search(no->right, code);
    }
    else
    {
        //se estiver no atual deu bommmmm
        printf("\nCodigo %d encontrado no indice %d!\n\n", no->key->code, no->key->RRN);

        printf("\n********* INFORMACOES DO FUNCIONARIO %d ENCONTRADO ********* !\n\n", no->key->code);

        FILE *arq_file = fopen("binario_arvore_b.dat", "rb+");

        //ativa funcao de ler
        TFuncc func = readFile(arq_file, no->key->RRN);

        leFuncc(func);

        fclose(arq_file);

        return 1;
    }
}


//imprime na tela as informa��es armazenadas nos n�s de uma �rvore bin�ria de busca ordenadas em ordem crescente de code.
//recebe um ponteiro para a raiz da �rvore como entrada
void toString(Node *root)
{
    if (root == NULL)
    {
        return;
    }
    else
    {
        //imprime da esquerda para a direita ordem crescenteee
        toString(root->left);
        printf("\nCode: %d\n", root->key->code
               );
        printf("Indice: %d\n", root->key->RRN);
        toString(root->right);
    }
}


//removeerr, mais facil do que parece kkkkkkk
//O par�metro root � a raiz da �rvore, e id � o c�digo do n� a ser removido vamos tirar um no
//quando chega no null tira e deixa desconexo
void remove_tree(Node *root, int id)
{
    //prox = pai
    Node *prox = NULL;
    Node *atual = root;

    //usa o while para percorrer ate achar ou nao kkkkkk
    while (atual != NULL)
    {
        // verifica se o n� atual � o n� a ser removido
        if (atual->key->code == id)
        {
            // verifica se o n� atual � o n� a ser removido no caso ele � uma folha
            if (atual->left == NULL && atual->right == NULL)
            {
                // verifica se o n� atual � o n� a ser removido
                if (prox == NULL)
                {
                    //retira o no
                    root = NULL;
                }
                else
                {
                    // se nao vai para esquerda ou direita verifica se o n� atual � o n� a ser removido
                    if (prox->left == atual)
                    {
                        prox->left = NULL;
                    }
                    else
                    {
                        prox->right = NULL;
                    }
                }

            }
             // se o n� a ser removido tem APENAS um filho,
            else if (atual->left == NULL)
            {
                // se o n� a ser removido � a raiz da �rvore
                if (prox == NULL)
                {
                    //filho substitui o n� a ser removido.
                    root = atual->right;
                }
                else
                {
                   // atualiza o pai do n� a ser removido para apontar para o seu filho � direita ou esquerda
                    if (prox->left == atual)
                    {
                        prox->left = atual->right;
                    }
                    else
                    {
                        prox->right = atual->right;
                    }
                }
            }
            // se o n� a ser removido tem DOISSSSS filhoss, primeiramente copia e cola o de cima
            else if (atual->right == NULL)
            {
                // se o n� a ser removido � a raiz da �rvor
                if (prox == NULL)
                {
                    root = atual->left;
                }
                else
                {
                   // atualiza o pai do n� a ser removido para apontar para o seu filho � direita ou esquerda
                    if (prox->left == atual)
                    {
                        prox->left = atual->left;
                    }
                    else
                    {
                        prox->right = atual->left;
                    }
                }
            }
            // se o n� a ser removido � a raiz da �rvor
            else
            {
                // encontra o sucessor do n� a ser removido (menor n� da sub�rvore � direita)
                Node *successor = atual->right;

                // para o pr�prio n� a ser removido
                Node *successor_parent = atual;



                // c�digo atualiza o ponteiro "successor_parent" para apontar para o ponteiro "successor" e atualiza o ponteiro "successor"
                //para apontar para o filho esquerdo do n� sucessor atual.
                while (successor->left != NULL)
                {
                    successor_parent = successor;
                    successor = successor->left;
                }

                // encontra o sucessor do n� a ser removido (menor n� da sub�rvore � direita)
                if (successor_parent->left == successor)
                {
                    successor_parent->left = successor->right;
                }
                else
                {
                    successor_parent->right = successor->right;
                }

                // substitui a chave do n� a ser removido pela chave do sucessor
                atual->key = successor->key;
            }

            // substitui a chave do n� a ser removido pela chave do sucessor
            free(atual);

            return;
        }

        //quando o n� atual n�o cont�m a chave de busca

       // Se o c�digo da chave do n� atual for maior do que o c�digo da chave de busca
       //significa que a chave de busca deve estar no sub�rvore esquerdo ou direito do n� atual
        else if (atual->key->code > id)
        {
            prox = atual;
            atual = atual->left;
        }
        else
        {
            prox = atual;
            atual = atual->right;
        }

    }//FIM DO WHILE

}


