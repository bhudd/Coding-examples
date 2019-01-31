#include <stdio.h>
#include <stdlib.h>

#include "fifo.h"

void printStr(char* str, int size)
{
    for(int i = 0; i < size; ++i)
    {
        printf("%02X ", str[i]);
    }
    printf("\n");
}

int main(void)
{
    fifo_t* fifo = malloc(sizeof(fifo_t));
    fifo_init(fifo, 10);

    printf("fifo init: %d %d %d\n", fifo->head, fifo->tail, fifo->size);

    int bytes;
    char b[] = {'1', '2', '3'};
    printStr(b, 3);
    bytes = put_data(fifo, b, sizeof(b));
    // printf("stored string: %s size: %d sizeof: %d\n", b, bytes, sizeof(b));
    char* c = malloc(4*sizeof(char));
    bytes = get_data(fifo, c, 3);
    c[3] = 0;

    printStr(c, 3);
    printf("retrieved string: %s size: %d\n", c, bytes);

    fifo_free(fifo);
}