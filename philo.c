#include "philo.h"

int	is_digit_str(char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	if (str[i] == '+')
		i++;
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

	if (!check_input(ac, av))
		return (printf("Invalid input\n"), 1);
	parse_args(ac, av, &data);
	if (init_mutexes(&data) || init_philos(&data) || start_threads(&data))
	{
		free_resources(&data);
		return (printf("Initialization error\n"), 1);
	}
	wait_threads(&data);
	free_resources(&data);
	return (0);
}

void	free_resources(t_data *data)
{
	int	i;

	if (data->philos != NULL)
	{
		i = 0;
		while (i < data->philo_num)
		{
			pthread_mutex_destroy(&data->philos[i].lock);
			i++;
		}
		free(data->philos);
		data->philos = NULL;
	}
	if (data->forks != NULL)
	{
		i = 0;
		while (i < data->philo_num)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
		data->forks = NULL;
	}
	pthread_mutex_destroy(&data->write);
	pthread_mutex_destroy(&data->lock);
}
