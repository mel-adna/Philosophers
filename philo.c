#include "philo.h"

int	is_digit_str(char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	check_input(int ac, char **av)
{
	int	i;
	int	philo_count;

	if (ac != 5 && ac != 6)
		return (0);
	i = 1;
	while (i < ac)
	{
		if (!is_digit_str(av[i]))
			return (0);
		i++;
	}
	philo_count = ft_atoi(av[1]);
	if (philo_count < 1 || philo_count > 200)
		return (0);
	if (ft_atoi(av[2]) <= 0 || ft_atoi(av[3]) <= 0 || ft_atoi(av[4]) <= 0)
		return (0);
	if (ac == 6 && ft_atoi(av[5]) < 0)
		return (0);
	return (1);
}

int	main(int ac, char **av)
{
	t_data	data;
	int		i;

	printf("Checking input...\n");
	if (!check_input(ac, av))
		return (printf("Invalid input\n"), 1);
	printf("Input OK\n");
	parse_args(ac, av, &data);
	if (init_mutexes(&data) || init_philos(&data) || start_threads(&data))
		return (printf("Initialization error\n"), 1);
	i = 0;
	while (i < data.philo_num)
		pthread_join(data.philos[i++].t1, NULL);
	return (0);
}
