// philo.h
#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	int				id;
	int				meals;
	long long		last_meal;
	pthread_t		thread;
	pthread_mutex_t	meal_mutex;
	struct s_data	*data;
}	t_philo;

typedef struct s_data
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_must_eat;
	int				dead;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	pthread_mutex_t start_mutex;
	pthread_mutex_t	dead_mutex;
	t_philo			*arr_philo;
}	t_data;

// Utility functions
long long	get_time(void);
void		ft_usleep(int ms);
void		print_status(t_philo *philo, char *status);

// Simulation functions
int			check_death(t_philo *philo);
int			simulation_stopped(t_data *data);
int			take_forks(t_philo *philo);
void		philo_eat(t_philo *philo);
void		*philosopher_routine(void *arg);
int			check_all_philosophers(t_philo *philos, t_data *data);

// Initialization functions
int			init_mutexes(t_data *data);
int			init_philosophers(t_data *data);
int			create_philosopher_threads(t_data *data);
void		join_philosopher_threads(t_data *data);
void		cleanup(t_data *data);

// Main simulation functions
int			handle_single_philosopher(t_data *data);
int			run_simulation(t_data *data);

// Argument parsing functions
int			nb_args(int argc);
int			is_valid_arguments(int argc, char **argv);
int			init_all(t_data *data, int argc, char **argv);
int			ft_atoi_safe(char *str);

#endif

// utils.c
#include "philo.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

void	ft_usleep(int ms)
{
	long long	start;

	start = get_time();
	while (get_time() - start < ms)
		usleep(500);
}

void	print_status(t_philo *philo, char *status)
{
	long long	timestamp;
	int			stopped;

	pthread_mutex_lock(&philo->data->print);
	pthread_mutex_lock(&philo->data->dead_mutex); 
	stopped = philo->data->dead;
	pthread_mutex_unlock(&philo->data->dead_mutex);
	if (!stopped)
	{
		timestamp = get_time() - philo->data->start_time;
		printf("%lld %d %s\n", timestamp, philo->id, status);
	}
	pthread_mutex_unlock(&philo->data->print);
}

void	cleanup(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->nb_philo)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
	}
	if (data->arr_philo)
	{
		i = 0;
		while (i < data->nb_philo)
		{
			pthread_mutex_destroy(&data->arr_philo[i].meal_mutex);
			i++;
		}
		free(data->arr_philo);
	}
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->dead_mutex);
	pthread_mutex_destroy(&data->start_mutex);
}

// parsing.c
#include "philo.h"

int	nb_args(int argc)
{
	if (argc < 5 || argc > 6)
	{
		printf("Usage: ./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat]\n");
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
	
	if (data->nb_philo <= 0 || data->time_to_die <= 0 || 
		data->time_to_eat <= 0 || data->time_to_sleep <= 0)
	{
		printf("Error: All values must be positive\n");
		return (1);
	}
	return (0);
}

// init.c
#include "philo.h"

int	init_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->forks)
		return (1);
	
	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
		{
			while (--i >= 0)
				pthread_mutex_destroy(&data->forks[i]);
			free(data->forks);
			return (1);
		}
		i++;
	}
	
	if (pthread_mutex_init(&data->print, NULL))
		return (1);
	if (pthread_mutex_init(&data->dead_mutex, NULL)) 
		return (1);
	if (pthread_mutex_init(&data->start_mutex, NULL))
    	return (1);
	return (0);
}

int	init_philosophers(t_data *data)
{
	int	i;

	data->arr_philo = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->arr_philo)
		return (1);
	
	i = 0;
	while (i < data->nb_philo)
	{	
		data->arr_philo[i].id = i + 1;
		data->arr_philo[i].meals = 0;
		data->arr_philo[i].data = data;
		data->arr_philo[i].last_meal = data->start_time;
		if (pthread_mutex_init(&data->arr_philo[i].meal_mutex, NULL))
			return (1);
		i++;
	}
	return (0);
}

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

// monitor.c
#include "philo.h"

int check_death(t_philo *philo)
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

int check_all_philosophers(t_philo *philos, t_data *data)
{
    int i;
    
    i = 0;
    while (i < data->nb_philo)
    {
        if (check_death(&philos[i]))
            return (1);
        i++;
    }
    
    if (data->nb_must_eat <= 0)
        return (0);
    
    i = 0;
    while (i < data->nb_philo)
    {
        pthread_mutex_lock(&philos[i].meal_mutex);
        if (philos[i].meals < data->nb_must_eat)
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

// routine.c
#include "philo.h"

int	simulation_stopped(t_data *data)
{
	int val;
	
	pthread_mutex_lock(&data->dead_mutex);
	val = data->dead;
	pthread_mutex_unlock(&data->dead_mutex);
	return (val);
}

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

// main.c
#include "philo.h"

int	run_simulation(t_data *data)
{
	if (init_mutexes(data)) 
        return (1);
	if (init_philosophers(data)) 
    {   
        cleanup(data); 
        return (1); 
    }

	pthread_mutex_lock(&data->start_mutex);
	data->start_time = get_time();

    int i = 0;
	while (i < data->nb_philo)
	{	
        data->arr_philo[i].last_meal = data->start_time;
        i++;
    }
    
	if (create_philosopher_threads(data)) 
    {
        cleanup(data); 
        return (1); 
    }

	pthread_mutex_unlock(&data->start_mutex);

	while (!simulation_stopped(data))
	{
		if (check_all_philosophers(data->arr_philo, data))
			break;
		ft_usleep(1);
	}
	
	join_philosopher_threads(data);
	cleanup(data);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (nb_args(argc))
		return (1);
	if (is_valid_arguments(argc, argv))
		return (1);
	if (init_all(&data, argc, argv))
		return (1);
	if (handle_single_philosopher(&data))
		return (0);
	return (run_simulation(&data));
}