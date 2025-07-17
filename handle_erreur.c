/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_erreur.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aregragu <aregragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 23:36:55 by aregragu          #+#    #+#             */
/*   Updated: 2025/07/17 23:37:09 by aregragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	handle_single_philosopher(t_data *data)
{
	if (data->nb_philo == 1)
	{
		printf("0 1 has taken a fork\n");
		ft_usleep(data->time_to_die, data);
		printf("%d 1 died\n", data->time_to_die);
		return (1);
	}
	return (0);
}
int check_all_philosophers(t_philo *philos, t_data *data) //return 1 si tous les philosophes ont mangé
{
    int i;
    
    i = 0;
    while (i < data->nb_philo)
    {
        if (check_death(&philos[i])) //return 1 si un philosophe est mort
            return (1);
        i++;
    }
    
    if (data->nb_must_eat <= 0)
        return (0);
    
    i = 0;
    while (i < data->nb_philo)
    {
        pthread_mutex_lock(&philos[i].meal_mutex); // Protéger l'accès à meals
        if (philos[i].meals < data->nb_must_eat) // Vérifier si le philosophe a mangé suffisamment
        {
            pthread_mutex_unlock(&philos[i].meal_mutex);
            return (0);
        }
        pthread_mutex_unlock(&philos[i].meal_mutex);
        i++;
    }
    
    pthread_mutex_lock(&data->dead_mutex);
    data->dead = 1;
    pthread_mutex_unlock(&data->dead_mutex);
    return (1);
}

int check_death(t_philo *philo) //return 1 si le philosophe est mort
{
    long long time_since_meal;
    
    pthread_mutex_lock(&philo->meal_mutex);
    time_since_meal = get_time() - philo->last_meal;
    pthread_mutex_unlock(&philo->meal_mutex);
    
    if (time_since_meal > philo->data->time_to_die)
    {
        pthread_mutex_lock(&philo->data->print);
        pthread_mutex_lock(&philo->data->dead_mutex);
        if (!philo->data->dead)
        {
            philo->data->dead = 1;
            printf("%lld %d died\n", get_time() - philo->data->start_time, philo->id);
        }
        pthread_mutex_unlock(&philo->data->dead_mutex);
        pthread_mutex_unlock(&philo->data->print);
        return (1);
    }
    return (0);
}

int	simulation_stopped(t_data *data) //return 1 si la simulation est arrêtée
{
    int val;
    
    pthread_mutex_lock(&data->dead_mutex);
    val = data->dead;
    pthread_mutex_unlock(&data->dead_mutex);
    return (val);
}
