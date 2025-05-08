#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (parse_input(argc, argv, &data))
		return (printf("Invalid input!\n"));
}
