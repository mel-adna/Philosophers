/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-adna <mel-adna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:00:00 by mel-adna          #+#    #+#             */
/*   Updated: 2025/05/11 16:25:04 by mel-adna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_data *data)
{
	pthread_mutex_lock(&data->lock);
	if (data->dead || (data->meals_nb != -1 && data->finished >= data->philo_num))
	{
		pthread_mutex_unlock(&data->lock);
		return (1);
	}
	pthread_mutex_unlock(&data->lock);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (!check_death(data))
	{
		pthread_mutex_lock(&data->lock);
		if (data->meals_nb != -1 && data->finished >= data->philo_num)
		{
			data->dead = 1;
			pthread_mutex_unlock(&data->lock);
			break;
		}
		pthread_mutex_unlock(&data->lock);
		usleep(1000);
	}
	return (NULL);
}
