#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_BOOKS 3

typedef struct {
    char title[50];
    char author[50];
    char status[10]; // "Available" or "Borrowed"
} Book;

Book books[NUM_BOOKS] = {
    {"Atomic Habits", "Mark Something", "Available"},
    {"You Deserve Each Other", "Sarah Hogle", "Borrowed"},
    {"Oliver Twist", "Charlie D", "Available"}
};

pthread_mutex_t book_mutex = PTHREAD_MUTEX_INITIALIZER;
int stop_execution = 0; // Global stop flag

void display_books() {
    printf("Book List:\n");
    for (int i = 0; i < NUM_BOOKS; i++) {
        printf("%s by %s - %s\n", books[i].title, books[i].author, books[i].status);
    }
    fflush(stdout); // Force output immediately
}

void *reader(void *arg) {
    int id = *(int *)arg;
    int read_count = 0;

    while (!stop_execution && read_count < 5) { // Limit each reader to 5 reads
        pthread_mutex_lock(&book_mutex);
        printf("Reader %d is reading book list:\n", id);
        display_books();
        pthread_mutex_unlock(&book_mutex);

        sleep(1);
        read_count++;
    }

    printf("Reader %d stopped.\n", id);
    fflush(stdout);
    return NULL;
}

void *writer(void *arg) {
    int id = *(int *)arg;
    int write_count = 0;

    while (!stop_execution && write_count < 3) { // Limit each writer to 3 updates
        pthread_mutex_lock(&book_mutex);

        int book_index = rand() % NUM_BOOKS;
        if (strcmp(books[book_index].status, "Available") == 0) {
            strcpy(books[book_index].status, "Borrowed");
        } else {
            strcpy(books[book_index].status, "Available");
        }

        printf("Writer %d updated book: %s, new status: %s\n",
               id, books[book_index].title, books[book_index].status);
        pthread_mutex_unlock(&book_mutex);

        sleep(2);
        write_count++;
    }

    printf("Writer %d stopped.\n", id);
    fflush(stdout);
    return NULL;
}
