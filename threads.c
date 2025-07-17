#include "philo.h"

int	create_philosopher_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_create(&data->arr_philo[i].thread, NULL, philosopher_routine, &data->arr_philo[i]))
			return (1);
		i++;
	}
	return (0);
}

void	join_philosopher_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_join(data->arr_philo[i].thread, NULL);
		i++;
	}
}
