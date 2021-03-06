/*
* @Author: alexandracandeias
* @Date:   2021-02-03 16:20:32
* @Last Modified by:   alexandracandeias
* @Last Modified time: 2021-02-13 16:32:19
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

typedef struct Lista
{
    int valor; //Corresponde ao índice da tabela
    char *caracter; //String padrão da tabela
    struct Lista *prox;
}Lista;

/**
 * @brief      Initialização da tabela de padrões com 256 padrões iniciais
 *
 * @return     Estutura inicializada
 */
Lista* initLista(){
    Lista *l = NULL;
    for (int i = 0; i<=256;i++) {
        Lista *aux = (Lista *) malloc(sizeof(Lista));
        aux -> valor = i;
        aux -> caracter = malloc(2);
        aux -> caracter[0] = i;
        aux -> caracter[1] = '\0';
        l =  adiciona(l,aux);
    }
    return l;
}

/**
 * @brief      Adiciona ao fim de uma lista, outra lista
 *
 * @param      l     lista inicial
 * @param      c     lista a adicionar
 *
 * @return     Lista alterada
 */
Lista* adiciona(Lista *l,Lista *c){
    Lista *aux = l;
    if(l == NULL) l = c;
    else{
        while (aux->prox) {
            aux = aux -> prox;
        }
        aux -> prox = c;
    }
    return l;
}

/**
 * @brief      Verifica se existe uma certa string na tabela de padrões
 *
 * @param      l     tabela de padrões
 * @param      s     string a verificar
 *
 * @return     1 caso exista, 0 caso contrário
 */
int existeString (Lista *l, char* s) {
	Lista *aux = l;
	while (aux) {
		if (strcmp(s, aux -> caracter) == 0) return 1;
		aux = aux -> prox;
	}
	return 0;
}

/**
 * @brief      Adiciona um padrão à tabela de padrôes
 *
 * @param      l     tabela de padrões
 * @param      s     string a adicionar
 * @param[in]  i     último índice
 */
void adicionarString (Lista *l, char* s, int i) {
	Lista *aux = l;
	while (aux -> prox) {
		aux = aux -> prox;
	}
	Lista *aux1 = (Lista *) malloc(sizeof(Lista));
	aux1 -> valor  =  i;
	aux1 -> prox = NULL;
	aux -> prox = aux1;
	aux1 -> caracter = malloc(strlen(s) + 2);
	strcpy(aux1 -> caracter, s); 
}

/**
 * @brief      Procura qual o índice correspondente a um determinado padrão
 *
 * @param      l     tabela de padrões
 * @param      s     string a verificar
 *
 * @return     Índice caso exista
 */
int caracterLookUp (Lista *l, char* s) {
    Lista *aux = l;
    while (aux){
        if (strcmp(s, aux -> caracter) == 0) return aux -> valor;
        aux = aux -> prox;
    }
    return -1;
}

/**
 * @brief      Liberta a memória ocupada pela lista
 *
 * @param      l     lista a apagar
 */
void destroyLista (Lista *l) {
    Lista *aux;
    while (l) {
        aux = l-> prox;
        free (l -> caracter);
        free (l);
        l = aux;
    }   
}


