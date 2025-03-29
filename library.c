#include "library.h"

//Book records
Book library[MAX_BOOKS] = {
    {"Atomic Habits", "Mark Something" , 1},
    {"You Deserve Each Other", "Sarah Hogle", 1},
    {"Oliver Twist", "Charlie D", 1}
};

//Initialisation

//Synchronisation of variables
pthread_mutex_t book_mutex;
sem_t rw_mutex;
int reader_count = 0;

//Mutex and Semaphores
void initialise_library(){
    pthread_mutex_init(&book_mutex, NULL);
    sem_init(&rw_mutex, 0 ,1);
}

//Reader Function
void *reader(void *arg){
    int id =*(int * )arg;
    while(1){
        //Reader count update
        pthread_mutex_lock(&book_mutex);
        reader_count++;
        if(reader_count == 1){
            sem_wait(&rw_mutex); //First reader blocks writers
        }
        pthread_mutex_unblock(&book_mutex);

        //Reading books
        printf("Reader %d is reading book list: \n", id);
        for(int i=0; i<MAX_BOOKS; i++){
            printf("%s by %s - %s\n", library[i].title, library[i].author, library[i].availaible ? "Available" : "Borrowed");
        }
        sleep(1); //simulating reading time

        //End of critical section (reader count update)
        pthread_mutex_lock(&book_mutex);
        reader_count--;
        if(reader_count == 0){
            sem_post(&rw_mutex); //Last reader allows writers
        }
        pthread_mutex_unblock(&book_mutex);
        sleep(rand() %3 +1);//simulating random reading intervals 
    }
    return NULL;
}

//Writer function(Librarians)
void *writer(void *arg){
    int id = *(int *)arg;

    while(1){
        sem_wait(&rw_mutex); //Writer gets exclusive access

        //Modify book records
        int book_index = rand() % MAX_BOOKS;
        library[book_index].availaible = !library[book_index].availaible;
        printf("Writer %d updated book: %s , new status: %s\n",id, library[book_index].title, library[book_index].availaible ? "Available" : "Borrowed");
        sleep(2); // Simulating writing time

        sem_post(&rw_mutex); //Release access for others
        sleep(rand() %5+1);//Simulating random writing intervals 

    }
    return NULL;
}