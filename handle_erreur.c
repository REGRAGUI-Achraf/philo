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
int simulation_has_ended(t_data *data)
{
	int has_ended_flag;

	pthread_mutex_lock(&data->print); // Le mutex 'print' protège 'data->dead'
	has_ended_flag = data->dead;
	pthread_mutex_unlock(&data->print);
	return (has_ended_flag);
}

void *monitor_routine(void *arg)
{
	t_data    *data;
	t_philo   *current_philo;
	int       i;
	long long time_now;
	long long local_last_philo_meal;
	int       local_philo_meals_count;
	int       all_philos_met_eat_target; // Vrai si tous les philos ont mangé nb_must_eat fois

	data = (t_data *)arg;

	while (1)
	{
		i = 0;
		// Par défaut, on suppose que l'objectif de repas est atteint,
		// sauf si on trouve un philosophe qui ne l'a pas encore fait.
		all_philos_met_eat_target = (data->nb_must_eat > 0); // Vrai seulement si nb_must_eat est actif

		while (i < data->nb_philo)
		{
			current_philo = &data->arr_philo[i];
			time_now = get_time();

			pthread_mutex_lock(&current_philo->meal_mutex);
			local_last_philo_meal = current_philo->last_meal;
			local_philo_meals_count = current_philo->meals;
			pthread_mutex_unlock(&current_philo->meal_mutex);

			// 1. VÉRIFIER LA MORT PAR FAMINE
			if ((time_now - local_last_philo_meal) > data->time_to_die)
			{
				pthread_mutex_lock(&data->print);
				if (!data->dead)
				{
					data->dead = 1;
					// Impression directe car print_status_safe ne s'imprimerait pas
					printf("%lld %d died\n", time_now - data->start_time, current_philo->id);
					pthread_mutex_unlock(&data->print);
					return (NULL); // Terminer le moniteur
				}
				pthread_mutex_unlock(&data->print);
				return (NULL); // Simulation déjà en arrêt
			}

			// 2. VÉRIFIER SI L'OBJECTIF DE REPAS EST ATTEINT (si nb_must_eat est défini)
			if (data->nb_must_eat > 0 && local_philo_meals_count < data->nb_must_eat)
			{
				all_philos_met_eat_target = 0; // Au moins un philo n'a pas fini
			}
			i++;
		}

		// 3. VÉRIFIER SI TOUS LES PHILOSOPHES ONT ATTEINT L'OBJECTIF DE REPAS
		if (data->nb_must_eat > 0 && all_philos_met_eat_target)
		{
			pthread_mutex_lock(&data->print);
			if (!data->dead)
			{
				data->dead = 1;
				// Optionnel: printf("%lld All philosophers have eaten enough.\n", get_time() - data->start_time);
				pthread_mutex_unlock(&data->print);
				return (NULL); // Terminer le moniteur
			}
			pthread_mutex_unlock(&data->print);
			return (NULL); // Simulation déjà en arrêt
		}

		// 4. SÉCURITÉ : Vérifier si 'data->dead' a été activé (pour sortir proprement)
		// Cette vérification peut être remplacée par un appel à simulation_has_ended()
		// si le moniteur doit s'arrêter dès que data->dead est 1, même s'il ne l'a pas mis lui-même.
		if (simulation_has_ended(data)) {
			return (NULL);
		}
		
		ft_usleep(1); // Petite pause (1ms par exemple)
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
		print_status(philo, "died");	
		philo->data->dead = 1;
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
