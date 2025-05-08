/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-adna <mel-adna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:35:08 by mel-adna          #+#    #+#             */
/*   Updated: 2025/05/08 13:11:16 by mel-adna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_data(t_data *data)
{
	data->philo_num = 0;
	data->death_time = 0;
	data->eat_time = 0;
	data->sleep_time = 0;
	data->meals_nb = -1;
	data->dead = 0;
	data->finished = 0;
	data->forks = NULL;
	data->philos = NULL;
	data->tid = NULL;
}

int	init_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (error("Mutex init failed (forks)", data));
		i++;
	}
	if (pthread_mutex_init(&data->write, NULL)
		|| pthread_mutex_init(&data->lock, NULL))
		return (error("Mutex init failed (synchronization)", data));
	return (0);
}

int	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
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
		i++;
	}
	return (0);
}
