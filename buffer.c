/*
* @Author: alexandracandeias
* @Date:   2021-02-03 16:30:37
* @Last Modified by:   alexandracandeias
* @Last Modified time: 2021-02-13 16:35:24
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct  buffer
{
	int valor; //Valor a ser impresso no output
	struct buffer *prox;
} Buffer;

/**
 * @brief      Adiciona um valor à cauda do buffer
 *
 * @param      b     buffer
 * @param[in]  i     valor a adicionar
 *
 * @return     Buffer alterado
 */
Buffer* adicionar(Buffer *b, int i) {
	if (b == NULL) 
	{
		b = malloc (sizeof(Buffer));
		b -> valor = i;
		b -> prox = NULL;
	}
	else {
		Buffer *aux = b;
		while (aux -> prox) {
			aux = aux -> prox;
		}
		Buffer *aux1 = malloc(sizeof(Buffer));
		aux1 -> valor = i;
		aux1 -> prox = NULL;
		aux -> prox = aux1;
	}
	return b;
}

/**
 * @brief      Calcula o tamanho do buffer
 *
 * @param      b     buffer
 *
 * @return     Tamanho do buffer
 */
int length(Buffer *b) {
	Buffer *aux = b;
	int contador = 0;
	while (aux) {
		aux = aux -> prox;
		contador = contador + 1;
	}
	return contador;
}

/**
 * @brief      Transforma o buffer num array de inteiros
 *
 * @param      b     buffer
 *
 * @return     Array de inteiros
 */
int* lista(Buffer *b) {
	int *lista = malloc(sizeof(int) * length(b));
	int i = 0;
	Buffer *aux = b;
	while (aux) {
		lista[i] = aux -> valor;
		aux = aux -> prox;
		i++;
	}
	return lista;
}

/**
 * @brief      Liberta a memória ocupada pelo buffer
 *
 * @param      b     buffer a apagar
 */
void destroyBuffer (Buffer *b) {
	Buffer *aux;
	while (b) {
		aux = b -> prox;
		free (b);
		b = aux;
	}
}


