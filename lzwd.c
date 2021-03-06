/*
* @Author: alexandracandeias
* @Date:   2021-02-02 17:41:47
* @Last Modified by:   alexandracandeias
* @Last Modified time: 2021-02-13 16:23:26
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "fsize.h"
#include "lista.h"
#include "buffer.h"

#define SIZE 65536

int max(int a,int b) {
    return a>b?a:b;
}

int min(int a, int b) {
    return a<b?a:b;
}

/**
 * @brief      função que retorna um apontador para uma string que é a junção de duas strings
 *
 * @param      a     primeira string
 * @param      b     segunda string
 *
 * @return     apontador para nova string
 */
char *juntaString(char *a,char *b){
    char *res = malloc(strlen(a) + strlen(b) + 1);
    int k = 0;
    for (int i = 0; i < strlen(a); i++, k++) {
        res[k] = a[i];
    }
    for (int j = 0; j < strlen(b); j++,k++) {
        res[k] = b[j];
    }
    res[k] = '\0';
    return res;
}

/**
 * @brief      função que a partir de uma string cria uma substring
 *
 * @param      s     string inicial
 * @param      sub   substring
 * @param[in]  p     posição inicial
 * @param[in]  l     tamanho da string
 */
void substring(char s[],char sub[],int p,int l){
    int i = 0;
    while (i<l){
        sub[i] = s[p+i-1];
        i++;
    }
    sub[i] = '\0';

}

int main(int argc, char **argv){
    int debug = 0; //Variável que indica se o output é debug ou não

    if (argc == 1) {
        printf("Ficheiro não fornecido \n");
        return 0;
    }

    if (strcmp (argv[1], "-d") == 0) debug = 1; 

    unsigned long long total;
    long long n_blocks;
    unsigned long size_of_last_block, block_size;
    FILE *fp,*saida;

    fp = fopen(argv[argc-1], "rb");
    if (debug == 0) saida = fopen(strcat(argv[argc-1], ".lzwd"), "w");
    else saida = fopen(strcat(argv[argc-1], ".lzwd.d"), "w");

    if (fp == NULL) {
        printf("Ficheiro inválido \n");
        return 0; 
    }

    block_size = SIZE;
    n_blocks = fsize(fp, NULL, &block_size, &size_of_last_block);
    total = (n_blocks-1) * block_size + size_of_last_block;
    //fclose(fp);
    
    fseek(fp,0,SEEK_SET);

    //Inicialização dos buffers de saída a NULL
    Buffer **buffers = malloc(n_blocks * sizeof(Buffer *));
    for (int i = 0; i < n_blocks; ++i) {
        buffers[i] = NULL;
    } 
    clock_t start = clock();

    //Inicialização da tabela de padrões com os 256 padrões inciais
    Lista *l = initLista();

    int last = 257; //Variável que indica qual a próxima posição a acrescentar na tabela de padrões

    //Ciclo para processamento por blocos
    for (int z = 0; z < n_blocks; z++){
        Buffer *b = buffers[z];
        char * bloco = NULL;

        //Posicionamento do ponteiro do ficheiro na posição correta
        fseek (fp, SIZE * z, SEEK_SET);

        //Alocamento e preenchimento do bloco com caracteres do ficheiro
        //Caso seja o último bloco, alocamos com tamanho size_of_last_block
        //Caso contrário, alocamos com tamanho 64Kbytes
        if (z == n_blocks-1) {
            bloco = malloc (size_of_last_block + 1);
            fread (bloco, sizeof(char), size_of_last_block, fp);
            bloco [size_of_last_block] = '\0';
        }
        else {
            bloco = malloc (SIZE + 1);
            fread (bloco, sizeof(char), SIZE, fp);
            bloco [SIZE] = '\0';
        }
        int n = 1; //Posição a partir da qual se vai ler do buffer de entrada
        char *pj = NULL, *pk = NULL;
        int sair = 0; //Booleano que indica se já chegou ao fim do buffer de entrada
        int size = 1; //Tamanho do maior padrão presente na tabela de padrões

        //Ciclo para processamento do bloco
        while(!sair){
            int oldPk; //Identificador do Pk antigo na tabela de padrões
            //j e k são variáveis que indicam se já foi encontrado o Pj e o Pk maiores, respetivamente
            //i corresponde ao tamanho a reduzir na leitura do buffer de entrada
            int j = 1,k = 1,i = 0; 

            //Se pk != NULL, guardar em oldPk o índice da sequência
            if (pk)
            {
                oldPk = caracterLookUp (l, pk);
                free (pk);
                pk = NULL;
            }

            //Calcula se pj que corresponde ao maior código presente na
            //tabela de sequências a partir da posição n, começando
            //a ver pelo tamanho da maior sequência na tabela e
            //diminuindo o tamanho até encontrar a sequência certa
            while (j) {
                    char *aux = malloc(size+1);
                    substring(bloco, aux, n, size-i);
                    if (existeString(l, aux)) {
                        pj = malloc(strlen(aux)+2);
                        strcpy(pj, aux);
                        j = 0;
                    } else {
                        i++;
                    }
                    free(aux);

            }
            i = 0;
            //Caso o size seja maior que o tamanho restante do buffer de entrada,
            //size passa a ser o tamanho restante do mesmo
            size = min (size, strlen (bloco) - n); 

            //Calcula se pk que corresponde ao maior código presente na
            //tabela de sequências a partir da posição n + (tamanho de pj), começando
            //a ver pelo tamanho da maior sequência na tabela (ou do maior possível dentro do bloco)
            //e diminuindo o tamanho até encontrar a sequência certa
            while (k){
                if(n+i>= strlen(bloco) || sair == 1){
                    sair = 1;
                    k = 0;
                }else{
                    char *aux = malloc(size+1);
                    substring(bloco,aux,n+strlen(pj),size-i);
                    if(existeString(l,aux) && strcmp(aux,"") != 0) {
                        pk = malloc(strlen(aux)+2);
                        strcpy(pk,aux);
                        n+=strlen(pj);
                        k = 0;
                    }
                    else{
                        i++;
                    }
                    free(aux);
                }
            }
            int oldPj = caracterLookUp (l, pj);
            char *pm = NULL;

            //Se pk != NULL, ou seja, ainda não chegou ao final do ficheiro,
            //pm passará a ser a junção das strings pj e pk
            if (pk)
            {
                pm = juntaString (pj, pk);
                adicionarString (l, pm, last++);
            }

            //Adicionar ao buffer de saída o índice de pj da tabela de sequências,
            //caso contrário, ou seja, caso se chegue ao fim do bloco, adiciona-se
            //o índice do pk anterior ao buffer de saída
            if(!sair || pk != NULL) {
                b = adicionar(b,oldPj);
                size = max(size,(int)strlen(pm));
            }
            else b = adicionar(b,oldPk);
            if(pk) size = max(size,(int)strlen(pk));
            free(pj);
            if(pm) free(pm);
        }
        buffers[z] = b;
        free (bloco);
    }

    clock_t stop = clock();

    //Imprime no ficheiro de saída, bloco a bloco, os resultados obtidos
    if (debug == 0)
    {
       for(int i = 0;i<n_blocks;i++) {
            int *numeros = lista (buffers[i]);
            for(int j = 0;j<length (buffers[i]);j++){
            fprintf(saida,"(%d)",numeros[j]);
        }   
       }
    }
    else {
        for(int i = 0;i<n_blocks;i++) {
            int *numeros = lista (buffers[i]);
            for(int j = 0;j<length (buffers[i]);j++){
            if (numeros[j] < 257) fprintf(saida,"('%c')",numeros[j]);
            else fprintf(saida,"(%d)",numeros[j]);
            } 
        }
    }
    
    fclose(saida);
    destroyLista(l);
    for (int i = 0; i < n_blocks; ++i) {
        destroyBuffer(buffers[i]);
    }

    time_t t = time (NULL);
    struct tm tm = *localtime (&t);

    printf("Alexandra Candeias, a89521, MIEI/CD, %02d-%02d-%d\n", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    printf("Número de blocos: %d\n", n_blocks);
    printf("Tempo de execução do módulo (ms): %f\n", (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC);
    if (debug == 0)
    {
         printf("Debug: Não\n");
         printf("Ficheiro gerado: %s\n", argv[argc - 1]);
    }
    else {
        printf("Debug: Sim\n");
        printf("Ficheiro gerado: %s\n", argv[argc - 1]);
    }

    return(0);
}
