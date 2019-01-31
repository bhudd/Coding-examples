#include "fifo.h"
#include <stdio.h>
#include <stdlib.h>

void fifo_init(fifo_t* fifo, int size)
{
    fifo->head = 0;
    fifo->tail = 0;
    fifo->size = size;
    fifo->buf = malloc(size*sizeof(char));
}

void test_endianness(void)
{
    uint16_t val = 1;
    uint8_t *vPtr;
    vPtr = (uint8_t*)&val;
    if(p[0] == 1)
        printf("little\n");
    else
        printf("big\n");
}

void fifo_free(fifo_t* fifo)
{
    free(fifo->buf);
    free(fifo);
}

int get_data(fifo_t* fifo, void* buf, int nbytes)
{
    char* p;

    p = buf;

    for(int i = 0; i < nbytes; ++i)
    {
        if(fifo->tail != fifo->head)
        {
            *p++ = fifo->buf[fifo->tail++];
            // check for wrap around
            if(fifo->tail == fifo->size) {
                fifo->tail = 0;
            }
        }
        else
        {
            return i;
        }
    }

    return nbytes;
}

int put_data(fifo_t* fifo, const void* buf, int nbytes)
{
    char const* p;

    p = buf;
    for(int i = 0; i < nbytes; ++i)
    {
        // check for room
        if((fifo->head + 1 == fifo->tail) ||
           (fifo->head + 1 == fifo->size && fifo->tail == 0))
        {
            // no more room
            return i;
        }
        else
        {
            fifo->buf[fifo->head] = *p++;
            fifo->head++;
            // check for wrap around
            if(fifo->head == fifo->size)
            {
                fifo->head = 0;
            }
        }
    }

    return nbytes;
}