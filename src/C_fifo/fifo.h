typedef struct
{
    char* buf;
    int head;
    int tail;
    int size;
} fifo_t;

void fifo_init(fifo_t* fifo, int size);
void fifo_free(fifo_t* fifo);
int get_data(fifo_t* fifo, void* buf, int nbytes);
int put_data(fifo_t* fifo, const void* buf, int nbytes);