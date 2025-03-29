#ifndef LIBRARY_H
#define LIBRARY_H

#include <pthread.h>

extern int stop_execution; // Global flag to stop execution

void *reader(void *arg);
void *writer(void *arg);

#endif // LIBRARY_H
