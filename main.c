/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-adna <mel-adna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:50:40 by mel-adna          #+#    #+#             */
/*   Updated: 2025/05/08 13:11:10 by mel-adna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

	i = 1;
	while (i < argc)
		if (!is_numeric(argv[i++]))
			return (error("Arguments must be numeric", NULL));
	data->philo_num = ft_atoi(argv[1]);
	data->death_time = (uint64_t)ft_atoi(argv[2]);
	data->eat_time = (uint64_t)ft_atoi(argv[3]);
	data->sleep_time = (uint64_t)ft_atoi(argv[4]);
	if (argc == 6)
		data->meals_nb = ft_atoi(argv[5]);
	else
		data->meals_nb = -1;
	if (data->philo_num < 1 || data->philo_num > 200 || data->death_time <= 0
		|| data->eat_time <= 0 || data->sleep_time <= 0 || (argc == 6
			&& data->meals_nb <= 0))
		return (error("Invalid arguments", data));
	data->dead = 0;
	data->finished = 0;
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
	i = 0;
	while (i < data->philo_num && !(pthread_create(&data->tid[i], NULL,
				&philo_routine, &data->philos[i])))
		i++;
	if (i < data->philo_num)
		return (error("Thread creation failed (philosopher)", data));
	i = 0;
	while (i < data->philo_num && !(pthread_join(data->tid[i++], NULL)))
		;
	if (i < data->philo_num)
		return (error("Thread join failed", data));
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	init_data(&data);
	if (argc < 5 || argc > 6)
		return (error("Usage: ./philo num_philos time_to_die time_to_eat time_to_sleep"
				" [num_times_each_must_eat]", NULL));
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
