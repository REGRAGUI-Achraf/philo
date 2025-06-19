#include "philo.h"

void	take_forks(t_philo *philo)
{
	int	first_fork;
	int	second_fork;
	int	left_fork = philo->id - 1;
	int	right_fork = philo->id % philo->data->nb_philo;

	if (left_fork < right_fork)
	{
		first_fork = left_fork;
		second_fork = right_fork;
	}
	else
	{
		first_fork = right_fork;
		second_fork = left_fork;
	}

	pthread_mutex_lock(&philo->data->forks[first_fork]);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->forks[second_fork]);
	print_status(philo, "has taken a fork");
}


void	philo_eat(t_philo *philo)
{
	int	left_fork;
	int	right_fork;

	left_fork = philo->id - 1;
	right_fork = philo->id % philo->data->nb_philo;
	take_forks(philo);
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time();
	philo->meals++;
	pthread_mutex_unlock(&philo->meal_mutex);
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(&philo->data->forks[left_fork]);
	pthread_mutex_unlock(&philo->data->forks[right_fork]);
}
int	simulation_stopped(t_data *data)
{
	int val;
	pthread_mutex_lock(&data->dead_mutex);
	val = data->dead;
	pthread_mutex_unlock(&data->dead_mutex);
	return val;
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;
	int		meals_count;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		ft_usleep(philo->data->time_to_eat / 2);
	while (!simulation_stopped(philo->data))
	{
		philo_eat(philo);
		pthread_mutex_lock(&philo->meal_mutex);
		meals_count = philo->meals;
		pthread_mutex_unlock(&philo->meal_mutex);
		if (philo->data->nb_must_eat > 0 && meals_count >= philo->data->nb_must_eat)
			break ;
		print_status(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep);
		print_status(philo, "is thinking");
	}
	return (NULL);
}
