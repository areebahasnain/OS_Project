// === main.c ===
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "library.h"
#include "dining.h"

#define NUM_READERS 3
#define NUM_WRITERS 2

char current_user[50] = "";

void show_main_menu();
void show_library_menu(int is_admin);
void run_simulation_mode();

int main() {
    initialize_library();
    
    while (1) {
        show_main_menu();
    }
    
    return 0;
}

void show_main_menu() {
    printf("\033[2J\033[H"); // clear screen
    printf("==========================================\n");
    printf("   Advanced Library Management System     \n");
    printf("==========================================\n\n");

    printf("Main Menu:\n");
    printf("1. Login\n");
    printf("2. Register\n");
    printf("3. Run Simulation Mode\n");
    printf("4. View Library as Guest\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");

    int choice;
    scanf("%d", &choice);

    switch(choice) {
        case 1: {
            char username[50], password[50];
            printf("Username: ");
            scanf("%s", username);
            printf("Password: ");
            scanf("%s", password);

            int auth_result = authenticate_user(username, password);
            if (auth_result > 0) {
                strcpy(current_user, username);
                printf("\nLogin successful. Welcome, %s!\n", username);
                sleep(1);
                show_library_menu(auth_result == 2);
            } else {
                printf("Invalid username or password.\n");
                sleep(2);
            }
            break;
        }
        case 2: {
            char username[50], password[50];
            printf("Choose a Username: ");
            scanf("%s", username);
            printf("Choose a Password: ");
            scanf("%s", password);

            if (register_user(username, password)) {
                printf("Registration successful! You can now login.\n");
            } else {
                printf("Registration failed. Username may already exist.\n");
            }
            sleep(2);
            break;
        }
        case 3:
            run_simulation_mode();
            break;
        case 4:
            pthread_mutex_lock(&book_mutex);
            display_books();
            pthread_mutex_unlock(&book_mutex);
            printf("\nPress Enter to return to Main Menu.");
            getchar(); getchar();
            break;
        case 5:
            printf("Exiting system. Goodbye!\n");
            exit(0);
        default:
            printf("Invalid choice. Try again.\n");
            sleep(2);
    }
}

void show_library_menu(int is_admin) {
    while (1) {
        printf("\033[2J\033[H");
        printf("\nLibrary Menu:\n");
        printf("1. View Available Books\n");
        printf("2. Borrow a Book\n");
        printf("3. Return a Book\n");
        if (is_admin) {
            printf("4. Add a Book\n");
            printf("5. Remove a Book\n");
            printf("6. Generate Report\n");
            printf("7. Logout\n");
        } else {
            printf("4. Logout\n");
        }
        printf("Enter your choice: ");

        int choice;
        scanf("%d", &choice);

        if ((!is_admin && choice == 4) || (is_admin && choice == 7)) {
            break;
        }

        switch(choice) {
            case 1:
                pthread_mutex_lock(&book_mutex);
                display_books();
                pthread_mutex_unlock(&book_mutex);
                printf("\nPress Enter to continue.");
                getchar(); getchar();
                break;
            case 2: {
                char title[50];
                printf("Enter book title to borrow: ");
                getchar();
                fgets(title, 50, stdin);
                title[strcspn(title, "\n")] = '\0';

                if (borrow_book(title, current_user)) {
                    printf("Book '%s' borrowed successfully.\n", title);
                } else {
                    printf("Failed to borrow book. It may not be available.\n");
                }
                sleep(2);
                break;
            }
            case 3: {
                char title[50];
                printf("Enter book title to return: ");
                getchar();
                fgets(title, 50, stdin);
                title[strcspn(title, "\n")] = '\0';

                if (return_book(title, current_user)) {
                    printf("Book '%s' returned successfully.\n", title);
                } else {
                    printf("Failed to return book.\n");
                }
                sleep(2);
                break;
            }
            case 4: case 5: case 6:
                if (is_admin) {
                    if (choice == 4) {
                        char title[50], author[50];
                        printf("Enter book title: ");
                        getchar();
                        fgets(title, 50, stdin);
                        title[strcspn(title, "\n")] = '\0';
                        printf("Enter author: ");
                        fgets(author, 50, stdin);
                        author[strcspn(author, "\n")] = '\0';

                        if (add_book(title, author)) {
                            printf("Book added successfully.\n");
                        } else {
                            printf("Failed to add book.\n");
                        }
                        sleep(2);
                    }
                    else if (choice == 5) {
                        char title[50];
                        printf("Enter book title to remove: ");
                        getchar();
                        fgets(title, 50, stdin);
                        title[strcspn(title, "\n")] = '\0';

                        if (remove_book(title)) {
                            printf("Book removed successfully.\n");
                        } else {
                            printf("Failed to remove book.\n");
                        }
                        sleep(2);
                    }
                    else if (choice == 6) {
                        generate_report();
                        printf("\nPress Enter to continue.");
                        getchar(); getchar();
                    }
                }
                break;
            default:
                printf("Invalid choice. Try again.\n");
                sleep(2);
        }
    }
}

void run_simulation_mode() {
    printf("\nSimulation Mode:\n");
    printf("1. Readers-Writers Problem\n");
    printf("2. Dining Philosophers Problem\n");
    printf("Enter your choice: ");

    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
        int reader_ids[NUM_READERS], writer_ids[NUM_WRITERS];

        printf("\nStarting Readers-Writers simulation...\n");
        stop_execution = 0;

        for (int i = 0; i < NUM_READERS; i++) {
            reader_ids[i] = i + 1;
            pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
        }
        for (int i = 0; i < NUM_WRITERS; i++) {
            writer_ids[i] = i + 1;
            pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
        }

        sleep(5); // Let it run
        stop_execution = 1;

        for (int i = 0; i < NUM_READERS; i++) {
            pthread_join(readers[i], NULL);
        }
        for (int i = 0; i < NUM_WRITERS; i++) {
            pthread_join(writers[i], NULL);
        }

        printf("\nReaders-Writers simulation completed.\n");
        printf("Press Enter to return to the main menu...");
        while (getchar() != '\n') {}
        getchar();
    } else if (choice == 2) {
        start_dining_philosophers();
    } else {
        printf("Invalid choice.\n");
    }
}
