#include "philo.h"

int	nb_args(int argc)
{
	if (argc < 5 || argc > 6)
	{
		printf("Usage ---->  ./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	return (0);
}
int	ft_atoi_safe(char *str)
{
	long	result;
	int		i;

	if (!str || !str[0])
		return (-1);
	
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		i++;
	}
	
	result = 0;
	i = 0;
	while (str[i])
	{
		result = result * 10 + (str[i] - '0');
		if (result > 2147483647)
			return (-1);
		i++;
	}
	
	if (result == 0)
		return (-1);
		
	return ((int)result);
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
