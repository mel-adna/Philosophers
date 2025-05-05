#include "philo.h"

void	parse_args(int ac, char **av, t_data *data)
{
	data->philo_num = ft_atoi(av[1]);
	data->death_time = (uint64_t)ft_atoi(av[2]);
	data->eat_time = (uint64_t)ft_atoi(av[3]);
	data->sleep_time = (uint64_t)ft_atoi(av[4]);
	data->meals_nb = -1;
	if (ac == 6)
		data->meals_nb = ft_atoi(av[5]);
	data->dead = 0;
	data->finished = 0;
	data->start_time = 0;
	data->philos = NULL;
	data->forks = NULL;
}
int	init_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_num);
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->philo_num)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_mutex_init(&data->write, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->lock, NULL) != 0)
		return (1);
	return (0);
}

int	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->philo_num);
	if (!data->philos)
		return (1);
	i = 0;
	while (i < data->philo_num)
	{
		data->philos[i].id = i + 1;
		data->philos[i].eat_count = 0;
		data->philos[i].data = data;
		data->philos[i].eating = 0;
		data->philos[i].status = 0;
		data->philos[i].time_to_die = 0;
		data->philos[i].l_fork = &data->forks[i];
		data->philos[i].r_fork = &data->forks[(i + 1) % data->philo_num];
		if (pthread_mutex_init(&data->philos[i].lock, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	start_threads(t_data *data)
{
	int			i;
	pthread_t	monitor_thread;

	data->start_time = get_time();
	i = 0;
	while (i < data->philo_num)
	{
		data->philos[i].time_to_die = get_time() + data->death_time;
		if (pthread_create(&data->philos[i].t1, NULL, routine,
				&data->philos[i]) != 0)
			return (1);
		// my_usleep(data, 1);
		i++;
	}
	if (data->philo_num > 1 && data->meals_nb > 0)
	{
		if (pthread_create(&monitor_thread, NULL, monitor, data) != 0)
			return (1);
		pthread_detach(monitor_thread);
	}
	return (0);
}

int	wait_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		if (pthread_join(data->philos[i].t1, NULL) != 0)
			return (1);
		if (data->philo_num > 1)
		{
			if (pthread_join(data->philos[i].supervisor, NULL) != 0)
				return (1);
		}
		i++;
	}
	return (0);
}
