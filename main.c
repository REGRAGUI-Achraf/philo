#include "philo.h"

int	run_simulation(t_data *data)
{
	pthread_t	monitor;

	data->start_time = get_time();
	if (init_mutexes(data))
		return (1);
	if (init_philosophers(data))
	{
		cleanup(data);
		return (1);
	}
	if (create_philosopher_threads(data))
	{
		cleanup(data);
		return (1);
	}
	if (pthread_create(&monitor, NULL, monitor_routine, data))
	{
		cleanup(data);
		return (1);
	}
	pthread_join(monitor, NULL);
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