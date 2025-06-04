#ifndef PHILO_H
# define PHILO_H


#include <pthread.h>
#include <stdio.h>

struct s_philo;



typedef struct s_data
{
    int             nb_philo;       // Nombre total de philosophes
    int             time_to_die;    // Temps (en ms) qu’un philosophe peut survivre sans manger
    int             time_to_eat;    // Temps (en ms) qu’un philosophe met à manger
    int             time_to_sleep;  // Temps (en ms) qu’un philosophe dort après avoir mangé
    int             nb_must_eat;    // Nombre de fois qu’un philosophe doit manger (optionnel)

    long long       start_time;     // Heure de départ du programme (en ms) pour calculer les timestamps
    int             dead;           // Flag indiquant si un philosophe est mort (1 = oui)

    pthread_mutex_t *forks;         // Tableau de mutex représentant les fourchettes (une par philosophe)
    pthread_mutex_t print;          // Mutex pour synchroniser l’affichage (évite les overlaps dans les logs)
    
    struct s_philo *arr_philo;
}               t_data;

typedef struct s_philo
{
    int             id;             // ID du philosophe (de 1 à nb_philo)
    int             meals;          // Nombre de repas déjà pris par ce philosophe
    long long       last_meal;      // Timestamp du dernier repas (en ms)

    pthread_t       thread;         // Thread du philosophe (son exécution parallèle)
    pthread_mutex_t meal_mutex;     // Mutex pour protéger l'accès à last_meal (éviter les data races)

    t_data          *data;          // Pointeur vers la struct globale (accès aux données partagées)
}               t_philo;

int	init_all(t_data *data, int ac, char **av);
int is_valid_arguments(int ac, char **av);
long	ft_atoi(const char *str);
int nb_args(int ac);



#endif