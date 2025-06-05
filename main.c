#include <stdio.h>
#include "philo.h"

int main(int ac, char **av)
{
    t_data data;       

    if (nb_args(ac) == -1)
        return 1;
    if (is_valid_arguments(ac, av) == -1)
        return 1;
    if (init_all(&data, ac, av) != 0)
    {
        printf("Erreur lors de l'initialisation.\n");
        return (1);
    }
    printf("%d / %d / %d / %d / %d", data.nb_philo, data.time_to_die, data.time_to_eat, data.time_to_sleep, data.nb_must_eat);
    return 0;
}
