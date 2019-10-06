// Author: Victor Murta
// Homework one for COMP 530: Operating Systems
// UNC Chapel Hill, Fall 2019
///////////////////////////////////////////////////////////////////////////////
// Description : Write a C program on Linux to read in a stream of characters
// from standard input (i.e., the keyboard) and write them as 80 character lines
// to standard output (i.e., the “screen”) with the following changes:
// • Every enter/return (newline) character in the input stream is replaced by a space
// • Every adjacent pair of asterisks “**” in the input stream is replaced by a “^”.
// now we also do this using pthreads and a custom buffer defined in buffer.h
#include "buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5
#define LINE_SIZE   5
Buffer read_newline, newline_asterisk, asterisk_write;


// just buffers input
void *processRead(void *arg){
    for(;;){
        deposit(&read_newline, getchar());
    }
    // don't have an exit condition, process terminates with 
    // the write thread
}

//converts newlines to spaces
void *processReturns(void *arg){
    char curChar;
    for (;;) {
        curChar = remoove(&read_newline);
        if (curChar == '\n'){
            curChar = ' ';
        }
        deposit(&newline_asterisk, curChar);
    }
    // don't have an exit condition, process terminates with 
    // the write thread
}

//thread that transforms pairs of asterisks into '^'
void *processAsterisks(void *arg){
    char curChar, nextChar;
    for(;;){
        curChar = remoove(&newline_asterisk);
        if (curChar == '*'){
            nextChar = remoove(&newline_asterisk);
            if (nextChar == '*'){
                deposit(&asterisk_write, '^');
            }
            else {
                deposit(&asterisk_write, curChar);
                deposit(&asterisk_write, nextChar);
            }
        }
        // non-asterisk case
        else {
            deposit(&asterisk_write, curChar);
        }
    }
    // don't have an exit condition, process terminates with 
    // the write thread
}

//fills a buffer with 80 characters then prints them
void *processWrite(void *arg){
    char line[LINE_SIZE];
    int index = 0;
    for (;;) {
        if (index == LINE_SIZE){
            puts(line);
            index = 0;
        }
        char curChar = remoove(&asterisk_write);
        if (curChar == -1){
            pthread_exit(NULL);
        }
        line[index] = curChar;
        index++;
    }
}

int main(){
    pthread_t   t_read, t_newline, t_asterisk, t_write;

    init_buff(BUFFER_SIZE, &read_newline);
    init_buff(BUFFER_SIZE, &newline_asterisk);
    init_buff(BUFFER_SIZE, &asterisk_write);

    pthread_create(&t_read,        NULL, &processRead,       NULL);
    pthread_create(&t_newline,     NULL, &processReturns,    NULL);
    pthread_create(&t_asterisk,    NULL, &processAsterisks,  NULL);
    pthread_create(&t_write,       NULL, &processWrite,      NULL);

    // just wait for the write thread to finish, then terminate the process
    pthread_join(t_write, NULL);

    delete_buff(&read_newline);
    delete_buff(&newline_asterisk);
    delete_buff(&asterisk_write);
}