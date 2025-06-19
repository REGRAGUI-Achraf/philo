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
	if (create_philosopher_threads(data))
	{
		cleanup(data);
		return (1);
	}
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