#include "philo.h"

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
		data->arr_philo[i].data = data;
		data->arr_philo[i].last_meal = data->start_time;
		if (pthread_mutex_init(&data->arr_philo[i].meal_mutex, NULL))
			return (1);
		i++;
	}
	return (0);
}

