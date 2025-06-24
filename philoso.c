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
	int left_fork = philo->id - 1;
	int right_fork = philo->id % philo->data->nb_philo;

	take_forks(philo);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time();
	philo->meals++;
	pthread_mutex_unlock(&philo->meal_mutex);

	print_status(philo, "is eating");
	ft_usleep(philo->data->time_to_eat);

	pthread_mutex_unlock(&philo->data->forks[left_fork]);
	pthread_mutex_unlock(&philo->data->forks[right_fork]);

	print_status(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
}
void	think_routine(t_philo *philo)
{
	long long	time_to_think;

	pthread_mutex_lock(&philo->meal_mutex);
	time_to_think = (philo->data->time_to_die - (get_time() - philo->last_meal) - philo->data->time_to_eat) / 2;
	pthread_mutex_unlock(&philo->meal_mutex);

	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think > 600)
		time_to_think = 200;
	print_status(philo, "is thinking");
	ft_usleep(time_to_think);
}
int	simulation_stopped(t_data *data)
{
	int val;
	pthread_mutex_lock(&data->dead_mutex);
	val = data->dead; // 1 si mort, 0 sinon
	pthread_mutex_unlock(&data->dead_mutex);
	return val;
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo = (t_philo *)arg;

	// 🔧 Synchronisation de départ
	pthread_mutex_lock(&philo->data->start_mutex);
	pthread_mutex_unlock(&philo->data->start_mutex);	

	if (philo->id % 2 == 0)
		ft_usleep(philo->data->time_to_eat);

	while (!simulation_stopped(philo->data))
	{
		philo_eat(philo);
		pthread_mutex_lock(&philo->meal_mutex);
		int meals_count = philo->meals;
		pthread_mutex_unlock(&philo->meal_mutex);
		if (philo->data->nb_must_eat > 0 && meals_count >= philo->data->nb_must_eat)
			break;
		think_routine(philo);
	}
	return NULL;
}
