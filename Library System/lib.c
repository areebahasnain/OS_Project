// === Fixed library.c ===
#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

Book books[MAX_BOOKS];
User users[MAX_USERS];
int book_count = 0;
int user_count = 0;
volatile int stop_execution = 0;

pthread_mutex_t book_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t user_mutex = PTHREAD_MUTEX_INITIALIZER;

void initialize_library() {
    add_book("Atomic Habits", "James Clear");
    add_book("You Deserve Each Other", "Sarah Hogle");
    add_book("Oliver Twist", "Charles Dickens");
    add_book("The Pragmatic Programmer", "Andrew Hunt");

    strcpy(users[0].username, "admin");
    strcpy(users[0].password, "admin123");
    users[0].is_admin = 1;

    strcpy(users[1].username, "user1");
    strcpy(users[1].password, "pass123");
    users[1].is_admin = 0;

    user_count = 2;
}

int authenticate_user(const char *username, const char *password) {
    pthread_mutex_lock(&user_mutex);
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0) {
            pthread_mutex_unlock(&user_mutex);
            return users[i].is_admin ? 2 : 1;
        }
    }
    pthread_mutex_unlock(&user_mutex);
    return 0;
}

int register_user(const char *username, const char *password) {
    pthread_mutex_lock(&user_mutex);
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            pthread_mutex_unlock(&user_mutex);
            return 0;
        }
    }
    if (user_count < MAX_USERS) {
        strcpy(users[user_count].username, username);
        strcpy(users[user_count].password, password);
        users[user_count].is_admin = 0;
        user_count++;
        pthread_mutex_unlock(&user_mutex);
        return 1;
    }
    pthread_mutex_unlock(&user_mutex);
    return 0;
}

void display_books() {
    printf("\nCurrent Library Status:\n");
    printf("------------------------------------------------------------------------------------------\n");
    printf("%-30s | %-20s | %-10s | %-15s | %-15s | Due Date\n", "Title", "Author", "Status", "Borrowed By", "Reserved By");
    printf("------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < book_count; i++) {
        char due_date_str[20] = "N/A";
        if (strcmp(books[i].status, "Borrowed") == 0) {
            strftime(due_date_str, 20, "%Y-%m-%d", localtime(&books[i].due_date));
        }
        printf("%-30s | %-20s | %-10s | %-15s | %-15s | %s\n",
               books[i].title, books[i].author, books[i].status,
               books[i].borrowed_by, books[i].reserved_by, due_date_str);
    }
    printf("------------------------------------------------------------------------------------------\n\n");
}

int count_user_borrowed_books(const char *username) {
    int count = 0;
    for (int i = 0; i < book_count; i++) {
        if (strcmp(books[i].borrowed_by, username) == 0)
            count++;
    }
    return count;
}

int add_book(const char *title, const char *author) {
    pthread_mutex_lock(&book_mutex);
    if (book_count >= MAX_BOOKS) {
        pthread_mutex_unlock(&book_mutex);
        return 0;
    }

    strcpy(books[book_count].title, title);
    strcpy(books[book_count].author, author);
    strcpy(books[book_count].status, "Available");
    strcpy(books[book_count].borrowed_by, "");
    strcpy(books[book_count].reserved_by, "");
    books[book_count].due_date = 0;
    book_count++;

    pthread_mutex_unlock(&book_mutex);
    return 1;
}

int borrow_book(const char *title, const char *username) {
    pthread_mutex_lock(&book_mutex);
    if (count_user_borrowed_books(username) >= MAX_BORROW_LIMIT) {
        pthread_mutex_unlock(&book_mutex);
        return 0;
    }

    for (int i = 0; i < book_count; i++) {
        if (strcmp(books[i].title, title) == 0) {
            if (strcmp(books[i].status, "Available") == 0) {
                strcpy(books[i].status, "Borrowed");
                strcpy(books[i].borrowed_by, username);
                books[i].due_date = time(NULL) + (14 * 24 * 60 * 60);
                pthread_mutex_unlock(&book_mutex);
                return 1;
            } else if (strcmp(books[i].reserved_by, "") == 0) {
                strcpy(books[i].reserved_by, username);
            }
            break;
        }
    }
    pthread_mutex_unlock(&book_mutex);
    return 0;
}

int return_book(const char *title, const char *username) {
    pthread_mutex_lock(&book_mutex);
    for (int i = 0; i < book_count; i++) {
        if (strcmp(books[i].title, title) == 0 &&
            strcmp(books[i].borrowed_by, username) == 0) {
            if (strcmp(books[i].reserved_by, "") != 0) {
                strcpy(books[i].borrowed_by, books[i].reserved_by);
                strcpy(books[i].reserved_by, "");
                books[i].due_date = time(NULL) + (14 * 24 * 60 * 60);
            } else {
                strcpy(books[i].status, "Available");
                strcpy(books[i].borrowed_by, "");
                books[i].due_date = 0;
            }
            pthread_mutex_unlock(&book_mutex);
            return 1;
        }
    }
    pthread_mutex_unlock(&book_mutex);
    return 0;
}

int remove_book(const char *title) {
    pthread_mutex_lock(&book_mutex);
    for (int i = 0; i < book_count; i++) {
        if (strcmp(books[i].title, title) == 0) {
            for (int j = i; j < book_count - 1; j++) {
                books[j] = books[j + 1];
            }
            book_count--;
            pthread_mutex_unlock(&book_mutex);
            return 1;
        }
    }
    pthread_mutex_unlock(&book_mutex);
    return 0;
}

void generate_report() {
    pthread_mutex_lock(&book_mutex);
    printf("\nLibrary Report:\n");
    printf("Total books: %d\n", book_count);

    int available = 0, borrowed = 0;
    for (int i = 0; i < book_count; i++) {
        if (strcmp(books[i].status, "Available") == 0)
            available++;
        else
            borrowed++;
    }
    printf("Available: %d\nBorrowed: %d\n", available, borrowed);

    time_t now = time(NULL);
    printf("\nOverdue Books:\n");
    for (int i = 0; i < book_count; i++) {
        if (strcmp(books[i].status, "Borrowed") == 0 &&
            books[i].due_date < now) {
            printf("- %s (Borrowed by %s, Due: %s)",
                   books[i].title, books[i].borrowed_by,
                   ctime(&books[i].due_date));
        }
    }
    pthread_mutex_unlock(&book_mutex);
}

void *reader(void *arg) {
    int id = *(int *)arg;
    srand(time(NULL) + id);

    while (!stop_execution) {
        pthread_mutex_lock(&book_mutex);
        printf("\n[Reader %d] Viewing books:\n", id);
        display_books();
        pthread_mutex_unlock(&book_mutex);
        sleep(1 + rand() % 3);
    }

    printf("[Reader %d] done.\n", id);
    return NULL;
}

void *writer(void *arg) {
    int id = *(int *)arg;
    srand(time(NULL) + id);

    while (!stop_execution) {
        pthread_mutex_lock(&book_mutex);
        int action = rand() % 3;

        if (action == 0 && book_count < MAX_BOOKS) {
            char title[50], author[50];
            sprintf(title, "New Book %d", rand() % 100);
            sprintf(author, "Author %d", rand() % 50);
            add_book(title, author);
            printf("[Writer %d] Added book: %s\n", id, title);
        } else if (action == 1 && book_count > 0) {
            int idx = rand() % book_count;
            strcpy(books[idx].status, "Available");
            strcpy(books[idx].borrowed_by, "");
            strcpy(books[idx].reserved_by, "");
            books[idx].due_date = 0;
            printf("[Writer %d] Returned book: %s\n", id, books[idx].title);
        } else if (action == 2 && book_count > 0) {
            int idx = rand() % book_count;
            printf("[Writer %d] Removed book: %s\n", id, books[idx].title);
            for (int j = idx; j < book_count - 1; j++) {
                books[j] = books[j + 1];
            }
            book_count--;
        }

        pthread_mutex_unlock(&book_mutex);
        sleep(2 + rand() % 2);
    }

    printf("[Writer %d] done.\n", id);
    return NULL;
}
