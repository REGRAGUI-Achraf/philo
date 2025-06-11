#include "philo.h"

int	check_all_philosophers(t_philo *philos, t_data *data)
{
	int	i;
	int	all_ate_enough;

	i = 0;
	all_ate_enough = 1;
	while (i < data->nb_philo)
	{
		if (check_death(&philos[i]))
			return (1);
		if (data->nb_must_eat > 0)
		{
			pthread_mutex_lock(&philos[i].meal_mutex);
			if (philos[i].meals < data->nb_must_eat)
				all_ate_enough = 0;
			pthread_mutex_unlock(&philos[i].meal_mutex);
		}
		else
			all_ate_enough = 0;
		i++;
	}
	if (all_ate_enough && data->nb_must_eat > 0)
	{
		data->dead = 1;
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (!simulation_stopped(data))
	{
		if (check_all_philosophers(data->arr_philo, data))
			return (NULL);
		ft_usleep(1);
	}
	return (NULL);
}
int	check_death(t_philo *philo)
{
	long long	time_since_meal;

	pthread_mutex_lock(&philo->meal_mutex);
	time_since_meal = get_time() - philo->last_meal;
	pthread_mutex_unlock(&philo->meal_mutex);
	if (time_since_meal > philo->data->time_to_die)
	{
		philo->data->dead = 1;
		print_status(philo, "died");
		return (1);
	}
	return (0);
}

int	simulation_stopped(t_data *data)
{
	return (data->dead);
}
int	handle_single_philosopher(t_data *data)
{
	if (data->nb_philo == 1)
	{
		printf("0 1 has taken a fork\n");
		ft_usleep(data->time_to_die);
		printf("%d 1 died\n", data->time_to_die);
		return (1);
	}
	return (0);
}
