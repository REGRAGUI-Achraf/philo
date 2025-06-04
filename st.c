#include <stdio.h>
#include <unistd.h>

void* incrementer(void* arg) {
    int compteur = 0;
    int thread_id = *(int*)arg;
    
    for (int i = 0; i < 100000; i++) {
        compteur++;
    }
    
    printf("Thread %d terminé\n", thread_id);
    return NULL;
}
