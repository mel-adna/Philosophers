#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
		return (printf("Wrong number of arguments"));
	if (parse_input(argc, argv, &data))
		return (1);
}
