#include "philo.h"

int	check_death(t_philo *philo) // Check if a philosopher has died
{
	long long	time_since_meal;

	pthread_mutex_lock(&philo->meal_mutex);
	time_since_meal = get_time() - philo->last_meal; // Calculate time since last meal
	pthread_mutex_unlock(&philo->meal_mutex);
	if (time_since_meal > philo->data->time_to_die)
	{
		print_status(philo, "died");
		pthread_mutex_lock(&philo->data->dead_mutex);
		philo->data->dead = 1; // Indicate that a philosopher has died
		pthread_mutex_unlock(&philo->data->dead_mutex);
		return (1);
	}
	return (0);
}

int check_all_philosophers(t_philo *philos, t_data *data)
{
    int i;
    
    i = 0;
    while (i < data->nb_philo)
    {
        if (check_death(&philos[i]))
            return (1);
        i++;
    }
    
    if (data->nb_must_eat <= 0)
        return (0);
    
    i = 0;
    while (i < data->nb_philo)
    {
        pthread_mutex_lock(&philos[i].meal_mutex);
        if (philos[i].meals < data->nb_must_eat)
        {
            pthread_mutex_unlock(&philos[i].meal_mutex);
            return (0); // Continuer la simulation
        }
        pthread_mutex_unlock(&philos[i].meal_mutex);
        i++;
    }
    pthread_mutex_lock(&data->dead_mutex);
    data->dead = 1;
    pthread_mutex_unlock(&data->dead_mutex);
    return (1);
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
