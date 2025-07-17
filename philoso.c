/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philoso.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aregragu <aregragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 23:36:49 by aregragu          #+#    #+#             */
/*   Updated: 2025/07/17 23:47:36 by aregragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	take_two_forks(t_philo *philo, int first, int second)
{
	pthread_mutex_lock(&philo->data->forks[first]);
	if (simulation_stopped(philo->data))
	{
		pthread_mutex_unlock(&philo->data->forks[first]);
		return (0);
	}
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->forks[second]);
	if (simulation_stopped(philo->data))
	{
		pthread_mutex_unlock(&philo->data->forks[second]);
		pthread_mutex_unlock(&philo->data->forks[first]);
		return (0);
	}
	print_status(philo, "has taken a fork");
	return (1);
}

int	take_forks(t_philo *philo)
{
	int	left;
	int	right;

	left = philo->id - 1;
	right = philo->id % philo->data->nb_philo;
	if (simulation_stopped(philo->data))
		return (0);
	if (philo->id % 2 == 0)
		return (take_two_forks(philo, left, right));
	else
		return (take_two_forks(philo, right, left));
}
void philo_eat(t_philo *philo)
{
	int left_fork = philo->id - 1;
	int right_fork = philo->id % philo->data->nb_philo;

	if (!take_forks(philo)) 
		return; // Simulation arrêtée

	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time();
	philo->meals++;
	pthread_mutex_unlock(&philo->meal_mutex);

	print_status(philo, "is eating");
	ft_usleep(philo->data->time_to_eat, philo->data);

	pthread_mutex_unlock(&philo->data->forks[left_fork]);
	pthread_mutex_unlock(&philo->data->forks[right_fork]);

	if (!simulation_stopped(philo->data)) 
	{
		print_status(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep, philo->data);
	}
}

void	think_routine(t_philo *philo)
{
	print_status(philo, "is thinking");
	
	if (philo->data->nb_philo % 2 != 0)
	{
		int think_time = (philo->data->time_to_eat * 2) - philo->data->time_to_sleep;
		if (think_time > 0)
			ft_usleep(think_time, philo->data);
	}
	else
	{
		ft_usleep(1, philo->data);
	}
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo = (t_philo *)arg;

	pthread_mutex_lock(&philo->data->start_mutex);
	pthread_mutex_unlock(&philo->data->start_mutex);

	if (philo->id % 2 == 0)
		ft_usleep(15, philo->data);

	while (!simulation_stopped(philo->data))
	{
		philo_eat(philo);
		
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

