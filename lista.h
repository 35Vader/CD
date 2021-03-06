#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

typedef struct Lista Lista;

Lista* initLista ();

Lista* adiciona (Lista *l, Lista *c);

int existeString (Lista *l, char* s);

void adicionarString (Lista *l, char* s, int i);

int caracterLookUp (Lista *l, char* s);

void destroyLista (Lista *l);

#endif // LISTA_H_INCLUDED
