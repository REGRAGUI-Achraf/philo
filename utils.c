#include "philo.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}
void	ft_usleep(int ms)
{
	long long	start;

	start = get_time();
	while (get_time() - start < ms)
		usleep(100);
}
void	print_status(t_philo *philo, char *status)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->data->print);
	if (!philo->data->dead)
	{
		timestamp = get_time() - philo->data->start_time;
		printf("%lld %d %s\n", timestamp, philo->id, status);
	}
	pthread_mutex_unlock(&philo->data->print);
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->arr_philo[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&data->print);
	free(data->forks);
	free(data->arr_philo);
}
