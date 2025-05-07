/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-adna <mel-adna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 18:44:40 by mel-adna          #+#    #+#             */
/*   Updated: 2025/05/07 18:44:41 by mel-adna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_data *data)
{
	int	result;

	pthread_mutex_lock(&data->lock);
	result = data->dead;
	pthread_mutex_unlock(&data->lock);
	return (result);
}

void	*monitor_routine(void *arg)
{
	t_data *data;

	data = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->lock);
		if (data->dead)
		{
			pthread_mutex_unlock(&data->lock);
			break ;
		}
		if (data->meals_nb != -1 && data->finished >= data->philo_num)
		{
			data->dead = 1;
			pthread_mutex_unlock(&data->lock);
			break ;
		}
		pthread_mutex_unlock(&data->lock);
		usleep(1000);
	}
	return (NULL);
}
