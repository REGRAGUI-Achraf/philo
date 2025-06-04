#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* ma_fonction_thread(void* arg) 
{
    sleep(3);
    printf("Thread exécuté!\n");
    return NULL;
}

int main() {
    pthread_t mon_thread;
    int valeur = 42;
    
    int resultat = pthread_create(&mon_thread, NULL, ma_fonction_thread, NULL);
    
    if (resultat == 0) 
    {
        // printf("J'attends que le thread se termine...\n");
        // pthread_join(mon_thread, NULL);
        pthread_detach(mon_thread);
        printf("Thread détaché, je continue sans attendre\n");
    } 
    else 
        printf("Erreur lors de la création du thread\n");    
    return 0;
}