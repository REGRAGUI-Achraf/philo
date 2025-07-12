#include "philo.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

void	ft_usleep(int ms, t_data *data)
{
	long long	start;
	long long	current;

	start = get_time();
	while (!simulation_stopped(data))
	{
		current = get_time();
		if (current - start >= ms)
			break;
		usleep(100);
	}
}

void	print_status(t_philo *philo, char *status)
{
	long long	timestamp;
	int			stopped;

	pthread_mutex_lock(&philo->data->print);
	pthread_mutex_lock(&philo->data->dead_mutex); 
	stopped = philo->data->dead;
	pthread_mutex_unlock(&philo->data->dead_mutex);
	if (!stopped)
	{
		timestamp = get_time() - philo->data->start_time;
		printf("%lld %d %s\n", timestamp, philo->id, status);
	}
	pthread_mutex_unlock(&philo->data->print);
}

void	cleanup(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->nb_philo)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
	}
	if (data->arr_philo)
	{
		i = 0;
		while (i < data->nb_philo)
		{
			pthread_mutex_destroy(&data->arr_philo[i].meal_mutex);
			i++;
		}
		free(data->arr_philo);
	}
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->dead_mutex);
	pthread_mutex_destroy(&data->start_mutex);
}