/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-adna <mel-adna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 18:44:36 by mel-adna          #+#    #+#             */
/*   Updated: 2025/05/07 18:47:16 by mel-adna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int						i;
	int						sign;
	unsigned long			result;

	i = 0;
	result = 0;
	sign = 1;
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
		if (result > 9223372036854775807 && sign > 0)
			return (-1);
		if (result > 9223372036854775807 && sign < 0)
			return (0);
	}
	return (result * sign);
}

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	parse_args(int argc, char **argv, t_data *data)
{
	int	i;

	for (i = 1; i < argc; i++)
		if (!is_numeric(argv[i]))
			return (error("Arguments must be numeric", NULL));
	data->philo_num = ft_atoi(argv[1]);
	data->death_time = (uint64_t)ft_atoi(argv[2]);
	data->eat_time = (uint64_t)ft_atoi(argv[3]);
	data->sleep_time = (uint64_t)ft_atoi(argv[4]);
	data->meals_nb = (argc == 6) ? ft_atoi(argv[5]) : -1;
	if (data->philo_num < 1 || data->philo_num > 200 || data->death_time <= 0
		|| data->eat_time <= 0 || data->sleep_time <= 0 || (argc == 6
			&& data->meals_nb <= 0))
		return (error("Invalid arguments", data));
	data->dead = 0;
	data->finished = 0;
	return (0);
}

int	init_mutexes(t_data *data)
{
	int	i;

	for (i = 0; i < data->philo_num; i++)
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (error("Mutex init failed (forks)", data));
	if (pthread_mutex_init(&data->write, NULL)
		|| pthread_mutex_init(&data->lock, NULL))
		return (error("Mutex init failed (synchronization)", data));
	return (0);
}

int	init_philos(t_data *data)
{
	int	i;

	for (i = 0; i < data->philo_num; i++)
	{
		data->philos[i].id = i + 1;
		data->philos[i].data = data;
		data->philos[i].eat_count = 0;
		data->philos[i].is_full = 0;
		data->philos[i].eating = 0;
		data->philos[i].time_to_die = get_time() + data->death_time;
		data->philos[i].r_fork = &data->forks[i];
		data->philos[i].l_fork = &data->forks[(i + 1) % data->philo_num];
		if (pthread_mutex_init(&data->philos[i].lock, NULL))
			return (error("Mutex init failed (philo lock)", data));
	}
	return (0);
}

int	create_threads(t_data *data)
{
	int			i;
	pthread_t	monitor_tid;

	data->tid = malloc(sizeof(pthread_t) * data->philo_num);
	if (!data->tid)
		return (error("Malloc failed (tid)", data));
	data->start_time = get_time();
	if (data->meals_nb != -1)
	{
		if (pthread_create(&monitor_tid, NULL, &monitor_routine, data))
			return (error("Thread creation failed (monitor)", data));
		pthread_detach(monitor_tid);
	}
	for (i = 0; i < data->philo_num; i++)
	{
		if (pthread_create(&data->tid[i], NULL, &philo_routine,
				&data->philos[i]))
			return (error("Thread creation failed (philosopher)", data));
	}
	for (i = 0; i < data->philo_num; i++)
	{
		if (pthread_join(data->tid[i], NULL))
			return (error("Thread join failed", data));
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	data.tid = NULL;
	data.philo_num = 0;
	data.meals_nb = 0;
	data.dead = 0;
	data.finished = 0;
	data.philos = NULL;
	data.death_time = 0;
	data.eat_time = 0;
	data.sleep_time = 0;
	data.start_time = 0;
	data.forks = NULL;
	if (argc < 5 || argc > 6)
		return (error("Usage: ./philo num_philos time_to_die time_to_eat time_to_sleep"
						" [num_times_each_must_eat]",
						NULL));
	if (parse_args(argc, argv, &data))
		return (1);
	data.forks = malloc(sizeof(pthread_mutex_t) * data.philo_num);
	if (!data.forks)
		return (error("Malloc failed (forks)", &data));
	data.philos = malloc(sizeof(t_philo) * data.philo_num);
	if (!data.philos)
		return (error("Malloc failed (philos)", &data));
	if (init_mutexes(&data))
		return (1);
	if (init_philos(&data))
		return (1);
	if (create_threads(&data))
		return (1);
	ft_exit(&data);
	return (0);
}
