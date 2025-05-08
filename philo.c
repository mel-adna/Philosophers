/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-adna <mel-adna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:50:21 by mel-adna          #+#    #+#             */
/*   Updated: 2025/05/08 16:05:50 by mel-adna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->data->write);
	if (!philo->data->dead)
		printf("%llu %d %s\n", (unsigned long long)(get_time()
				- philo->data->start_time), philo->id, status);
	pthread_mutex_unlock(&philo->data->write);
}

static void	philo_eat(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->r_fork < philo->l_fork)
	{
		first_fork = philo->r_fork;
		second_fork = philo->l_fork;
	}
	else
	{
		first_fork = philo->l_fork;
		second_fork = philo->r_fork;
	}
	pthread_mutex_lock(first_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(second_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(&philo->lock);
	philo->eating = 1;
	philo->time_to_die = get_time() + philo->data->death_time;
	pthread_mutex_unlock(&philo->lock);
	print_status(philo, "is eating");
	ft_usleep(philo->data->eat_time);
	pthread_mutex_lock(&philo->lock);
	philo->eat_count++;
	philo->eating = 0;
	pthread_mutex_unlock(&philo->lock);
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
}

static void	*supervisor(void *arg)
{
	t_philo		*philo;
	uint64_t	current_time;

	philo = (t_philo *)arg;
	while (1)
	{
		if (check_death(philo->data))
			break ;
		current_time = get_time();
		pthread_mutex_lock(&philo->lock);
		if (!philo->eating && current_time > philo->time_to_die)
		{
			pthread_mutex_lock(&philo->data->lock);
			if (!philo->data->dead)
			{
				philo->data->dead = 1;
				pthread_mutex_unlock(&philo->data->lock);
				pthread_mutex_lock(&philo->data->write);
				printf("%llu %d died\n", (unsigned long long)(current_time
						- philo->data->start_time), philo->id);
				pthread_mutex_unlock(&philo->data->write);
			}
			else
				pthread_mutex_unlock(&philo->data->lock);
			pthread_mutex_unlock(&philo->lock);
			return (NULL);
		}
		if (philo->data->meals_nb != -1
			&& philo->eat_count >= philo->data->meals_nb && !philo->is_full)
		{
			pthread_mutex_lock(&philo->data->lock);
			philo->is_full = 1;
			philo->data->finished++;
			pthread_mutex_unlock(&philo->data->lock);
		}
		pthread_mutex_unlock(&philo->lock);
		usleep(100);
	}
	return (NULL);
}

void	*philo_routine(void *arg)
{
	t_philo		*philo;
	pthread_t	tid;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->lock);
	philo->time_to_die = get_time() + philo->data->death_time;
	pthread_mutex_unlock(&philo->lock);
	if (pthread_create(&tid, NULL, &supervisor, philo))
		return (NULL);
	pthread_detach(tid);
	if (philo->data->philo_num == 1)
	{
		print_status(philo, "has taken a fork");
		while (!check_death(philo->data))
			usleep(1000);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		ft_usleep(philo->data->eat_time / 2);
	while (!check_death(philo->data) && (philo->data->meals_nb == -1
			|| philo->eat_count < philo->data->meals_nb))
	{
		philo_eat(philo);
		print_status(philo, "is sleeping");
		ft_usleep(philo->data->sleep_time);
		print_status(philo, "is thinking");
		usleep(150);
	}
	return (NULL);
}
