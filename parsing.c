#include "philo.h"

int	nb_args(int argc)
{
	if (argc < 5 || argc > 6)
	{
		printf("TRY: ./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	return (0);
}

int	ft_atoi_safe(char *str)
{
	int		i;
	int		sign;
	long	n;

	if (!str)
		return (0);
	i = 0;
	sign = 1;
	n = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	i = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		n = n * 10 + (str[i] - '0');
		i++;
	}
	return ((int)(n * sign));
}

int	is_valid_arguments(int argc, char **argv)
{
	int	i;
	int	value;

	i = 1;
	while (i < argc)
	{
		value = ft_atoi_safe(argv[i]);
		if (value <= 0)
		{
			printf("Error: All arguments must be positive integers\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_all(t_data *data, int argc, char **argv)
{
	data->nb_philo = ft_atoi_safe(argv[1]);
	data->time_to_die = ft_atoi_safe(argv[2]);
	data->time_to_eat = ft_atoi_safe(argv[3]);
	data->time_to_sleep = ft_atoi_safe(argv[4]);
	if (argc == 6)
		data->nb_must_eat = ft_atoi_safe(argv[5]);
	else
		data->nb_must_eat = -1;
	data->dead = 0;
	data->start_time = 0;
	data->forks = NULL;
	data->arr_philo = NULL;
	if (data->nb_philo <= 0 || data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0)
	{
		printf("Error: All values must be positive\n");
		return (1);
	}
	return (0);
}
