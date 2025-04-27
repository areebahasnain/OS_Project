// library.h
#ifndef LIBRARY_H
#define LIBRARY_H

#include <pthread.h>

#define MAX_USERS 10
#define MAX_BOOKS 50

typedef struct {
    char username[50];
    char password[50];
    int is_admin;
} User;

typedef struct {
    char title[50];
    char author[50];
    char status[20]; // "Available", "Borrowed", "Reserved"
    char borrowed_by[50];
    time_t due_date;
} Book;

extern Book books[MAX_BOOKS];
extern User users[MAX_USERS];
extern int book_count;
extern int user_count;
extern int stop_execution;
extern pthread_mutex_t book_mutex;
extern pthread_mutex_t user_mutex;

void initialize_library();
void *reader(void *arg);
void *writer(void *arg);
void display_books();
int authenticate_user(const char *username, const char *password);
int add_book(const char *title, const char *author);
int remove_book(const char *title);
int borrow_book(const char *title, const char *username);
int return_book(const char *title);
void generate_report();

#endif // LIBRARY_H