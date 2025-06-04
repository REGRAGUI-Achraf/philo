#include "philo.h"
#include <limits.h>
#include <stdio.h>

int	nb_args(int ac)
{
	if (ac < 5 || ac > 6)
	{
		if (ac < 5)
			printf("Not enough arguments\n");
		else
			printf("Too many arguments\n");
		return (-1);
	}
	return (0);
}

long	ft_atoi(const char *str)
{
	int		i;
	long	n;

	i = 0;
	n = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		n = n * 10 + (str[i] - '0');
		if (n > INT_MAX)
			return (-1);
		i++;
	}
	return (n);
}

int	is_valid_arguments(int ac, char **av)
{
	int		i;
	long	value;

	i = 1;
	while (i < ac)
	{
		value = ft_atoi(av[i]);
		if (value <= 0)
		{
			printf("Invalid value in argument\n");
			return (-1);
		}
		i++;
	}
	return (0);
}
