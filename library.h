#ifndef LIBRARY_H
#define LIBRARY_H   

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>


#define MAX_READERS 5
#define MAX_BOOKS 3

//Book Structure

typedef struct {
    char title[50];
    char author[50];
    int available; //1 if available , 0 if borrowed

} Book;

extern Book library[MAX_BOOKS];
extern pthread_mutex_t book_mutex;
extern sem_t rw_mutex;
extern int reader_count;

//Functions 
void *reader(void *arg);
void *writer(void *arg);
void initilise_library();

#endif