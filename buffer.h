#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

typedef struct  buffer Buffer;

Buffer* adicionar(Buffer *b, int i);

int length(Buffer *b);

int* lista(Buffer *b);

void destroyBuffer (Buffer *b);

#endif // BUFFER_H_INCLUDED