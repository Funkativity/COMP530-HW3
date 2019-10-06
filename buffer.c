#include "buffer.h"

void init_buff(int buffer_size, Buffer *buff){
    buff->buff_size = buffer_size;
    buff->next_in = 0;
    buff->next_out = 0;
    assert(sem_init(&(buff->full_slots), 0, 0) == 0);
    assert(sem_init(&(buff->empty_slots), 0, buffer_size) == 0);
    buff->buff = (char*) malloc(buffer_size * sizeof(char));

    
}

// returns the the next item in the buffer
// sem locks until there is an unread item in buff
char remoove(Buffer *buff){
    char rt;
    
    
    assert(sem_wait(&(buff->full_slots)) == 0);
    //make sure that we have at least one buffer that is not full
    int num_full;
    sem_getvalue(&(buff->full_slots), &num_full);
    assert(num_full < buff->buff_size);

    rt = buff->buff[buff->next_out];
    buff->next_out = (buff->next_out + 1) % buff->buff_size;

    //make sure that we have at least one buffer that is empty
    int num_empty;
    sem_getvalue(&(buff->empty_slots), &num_empty);
    assert(num_empty >= 0);
    assert(sem_post(&(buff->empty_slots)) == 0);
    return rt;
}

// add a new element to the buffer
// sem locks until there is a free tem in the slot
void deposit(Buffer *buff, char new_char){
    assert(sem_wait(&(buff->empty_slots))==0);
    //make sure that we have at least one buffer that is empty
    int num_empty;
    sem_getvalue(&(buff->empty_slots), &num_empty);
    assert(num_empty >= 0);

    buff->buff[buff->next_in] = new_char;
    buff->next_in = (buff->next_in + 1) % buff->buff_size;

    //make sure that we have at least one buffer that is not full
    int num_full;
    sem_getvalue(&(buff->full_slots), &num_full);
    assert(num_full < buff->buff_size);

    assert(sem_post(&(buff->full_slots))==0);
    return;
}

void delete_buff(Buffer *buffer){
    sem_destroy(&(buffer->empty_slots));
    sem_destroy(&(buffer->empty_slots));
    free(buffer->buff);
    return;
}