#include "philo.h"

int	init_all(t_data *data, int argc, char **argv)
{
	data->nb_philo = ft_atoi_safe(argv[1]);
	data->time_to_die = ft_atoi_safe(argv[2]);
	data->time_to_eat = ft_atoi_safe(argv[3]);
	data->time_to_sleep = ft_atoi_safe(argv[4]);
	
	if (argc == 6)
		data->nb_must_eat = ft_atoi_safe(argv[5]);
	else
		data->nb_must_eat = -1;
	
	data->dead = 0;
	data->start_time = 0;
	data->forks = NULL;
	data->arr_philo = NULL;
	
	if (data->nb_philo <= 0 || data->time_to_die <= 0 || 
		data->time_to_eat <= 0 || data->time_to_sleep <= 0)
	{
		printf("Error: All values must be positive\n");
		return (1);
	}
	return (0);
}
int	init_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
		{
			while (--i >= 0)
				pthread_mutex_destroy(&data->forks[i]);
			free(data->forks);
			return (1);
		}
		i++;
	}
	if (pthread_mutex_init(&data->print, NULL))
		return (1);
	if (pthread_mutex_init(&data->dead_mutex, NULL)) 
		return (1);
	if (pthread_mutex_init(&data->start_mutex, NULL))
    	return (1);
	return (0);
}

int	init_philosophers(t_data *data)
{
	int	i;

	data->arr_philo = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->arr_philo)
		return (1);
	i = 0;
	while (i < data->nb_philo)
	{	
		data->arr_philo[i].id = i + 1;
		data->arr_philo[i].meals = 0;
		data->arr_philo[i].last_meal = data->start_time;
		data->arr_philo[i].data = data;
		if (pthread_mutex_init(&data->arr_philo[i].meal_mutex, NULL))
			return (1);
		i++;
	}
	return (0);
}