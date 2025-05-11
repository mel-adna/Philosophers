#include "philo.h"

int	main(int argc, char **argv)
{
	t_data data;

	if (argc < 5 || argc > 6)
		return (printf("Usage: ./philo num_philos time_to_die time_to_eat time_to_sleep"
						" [num_times_each_must_eat]"));
	if (parse_args(argc, argv, &data))
		return (1);
	return (0);
}
