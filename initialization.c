#include "philo.h"

int	init_all(t_data *data, int ac, char **av)
{
    data->dead = 0;
	data->nb_philo = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
    if (ac == 6)
        data->nb_must_eat = ft_atoi(av[5]);
    else
        data->nb_must_eat = -1;
    return (0);
    
}