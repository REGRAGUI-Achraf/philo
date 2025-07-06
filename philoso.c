#include "philo.h"

int take_forks(t_philo *philo)
{
    int left = philo->id - 1;
    int right = philo->id % philo->data->nb_philo;

    if (simulation_stopped(philo->data))
        return (0);

    if (philo->id % 2 == 0) {
        pthread_mutex_lock(&philo->data->forks[left]);
        if (simulation_stopped(philo->data)) {
            pthread_mutex_unlock(&philo->data->forks[left]);
            return (0);
        }
        print_status(philo, "has taken a fork");
        
        pthread_mutex_lock(&philo->data->forks[right]);
        if (simulation_stopped(philo->data)) {
            pthread_mutex_unlock(&philo->data->forks[right]);
            pthread_mutex_unlock(&philo->data->forks[left]);
            return (0);
        }
        print_status(philo, "has taken a fork");
    } else {
        pthread_mutex_lock(&philo->data->forks[right]);
        if (simulation_stopped(philo->data)) {
            pthread_mutex_unlock(&philo->data->forks[right]);
            return (0);
        }
        print_status(philo, "has taken a fork");
        
        pthread_mutex_lock(&philo->data->forks[left]);
        if (simulation_stopped(philo->data)) {
            pthread_mutex_unlock(&philo->data->forks[left]);
            pthread_mutex_unlock(&philo->data->forks[right]);
            return (0);
        }
        print_status(philo, "has taken a fork");
    }
    return (1);
}

void	philo_eat(t_philo *philo)
{
	int left_fork = philo->id - 1;
	int right_fork = philo->id % philo->data->nb_philo;

	// Mettre à jour last_meal AVANT de prendre les fourchettes
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->meal_mutex);

	if (!take_forks(philo)) {
		return; // Simulation arrêtée
	}

	// Mettre à jour les repas et le temps de repas final
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time();
	philo->meals++;
	pthread_mutex_unlock(&philo->meal_mutex);

	print_status(philo, "is eating");
	ft_usleep(philo->data->time_to_eat);

	pthread_mutex_unlock(&philo->data->forks[left_fork]);
	pthread_mutex_unlock(&philo->data->forks[right_fork]);

	if (!simulation_stopped(philo->data)) {
		print_status(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep);
	}
}

void	think_routine(t_philo *philo)
{
	long long	time_to_think;

	pthread_mutex_lock(&philo->meal_mutex);
	time_to_think = (philo->data->time_to_die - (get_time() - philo->last_meal) - philo->data->time_to_eat) / 2;
	pthread_mutex_unlock(&philo->meal_mutex);

	if (time_to_think < 0)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
		
	print_status(philo, "is thinking");
	
	long long start = get_time();
	while (get_time() - start < time_to_think && !simulation_stopped(philo->data))
		ft_usleep(1);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo = (t_philo *)arg;

	pthread_mutex_lock(&philo->data->start_mutex);
	pthread_mutex_unlock(&philo->data->start_mutex);	

	// Décalage pour éviter que tous mangent en même temps
	if (philo->id % 2 == 0)
		ft_usleep(philo->data->time_to_eat / 2);

	while (!simulation_stopped(philo->data))
	{
		philo_eat(philo);
		
		// Vérifier si on a assez mangé
		pthread_mutex_lock(&philo->meal_mutex);
		int meals_count = philo->meals;
		pthread_mutex_unlock(&philo->meal_mutex);
		
		if (philo->data->nb_must_eat > 0 && meals_count >= philo->data->nb_must_eat)
			break;
			
		if (simulation_stopped(philo->data))
			break;
			
		think_routine(philo);
	}
	return (NULL);
}