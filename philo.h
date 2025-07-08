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
	pthread_mutex_t	meal_mutex; // Mutex pour protéger l'accès à last_meal et meals
	struct s_data	*data;
}	t_philo;

typedef struct s_data
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_must_eat;
	int				dead; //0 si normal --- 1 si mort
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	pthread_mutex_t start_mutex;
	pthread_mutex_t	dead_mutex; // Ajouté pour protéger dead
	t_philo			*arr_philo;
}	t_data;

// Utility functions
long long	get_time(void);
void		ft_usleep(int ms);
void		ft_usleep_safe(t_philo *philo, int ms);

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

// Argument parsing functions (not defined in the provided code)
int			nb_args(int argc);
int			is_valid_arguments(int argc, char **argv);
int			init_all(t_data *data, int argc, char **argv);

int	ft_atoi_safe(char *str);


#endif