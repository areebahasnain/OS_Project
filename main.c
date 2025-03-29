#include "library.h"

int main() {
pthread_t readers[MAX_READERS], writers[2];
int reader_ids[MAX_READERS], writer_ids[2];

initialize_library(); // Initialize mutex & semaphores

// Create reader threads
for (int i = 0; i < MAX_READERS; i++) {
reader_ids[i] = i + 1;
pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
}

// Create writer threads
for (int i = 0; i < 2; i++) {
writer_ids[i] = i + 1;
pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
}

// Join threads (optional, for controlled termination)
for (int i = 0; i < MAX_READERS; i++) {
pthread_join(readers[i], NULL);
}
for (int i = 0; i < 2; i++) {
pthread_join(writers[i], NULL);
}

// Cleanup
pthread_mutex_destroy(&book_mutex);
sem_destroy(&rw_mutex);

return 0;
}

