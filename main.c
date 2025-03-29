int main(){
    pthread_t readers[MAX_READERS], writers[2]; //More readers than writers
    int reader_ids[MAX_READERS], writers_ids[2];

    pthread_mutex_init(&book_mutex, NULL);
    sem_init(&rw_mutex, 0,1);

    //Creating reader threads
    for (int i=0; i<MAX_READERS;i++){
        reader_ids[i] = i+1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    //Creating writer threads
    for (int i=0; i<2; i++){
    writer_ids[i] = i+1;
    pthreade_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    //Join threads for controlled termination
    for (int i = 0; i<MAX_READERS; i++){
        pthread_join(readers[i], NULL);
    }
    for (int i =0; i<2; i++){
        pthread_join(writers[i], NULL);
    }

    //Clean
    pthread_mutex_destroy(&book_mutex);
    sem_destroyed(&rw_mutex);
    return 0;
}//end main
