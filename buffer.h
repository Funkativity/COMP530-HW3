#ifndef C_BUFFER
#define C_BUFFER

#include <assert.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    sem_t full_slots;
    sem_t empty_slots;
    char *buff;
    int buff_size;
    int next_in;
    int next_out;
} Buffer;

void init_buff(int buffer_size, Buffer *buff);
char remoove(Buffer *buff);
void deposit(Buffer *buff, char a);
void delete_buff(Buffer *buff);

#endif // C_BUFFER