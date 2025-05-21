/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_lifecycle.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-adna <mel-adna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/30 18:11:11 by mbari             #+#    #+#             */
/*   Updated: 2025/05/21 10:19:21 by mel-adna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * Thread function to monitor philosopher's health status
 * 
 * @param arg Pointer to philosopher structure
 * @return NULL
 */
void	*monitor_philosopher_health(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!philo->is_dead)
	{
		sem_wait(philo->data->death);
		// Check if philosopher has died
		if (philo->next_meal < get_timestamp_ms())
		{
			print_status(STATE_DIED, philo);
			philo->is_dead = TRUE;
			sem_post(philo->data->stop);
			sem_post(philo->data->death);
			break;
		}
		sem_post(philo->data->death);
		
		sem_wait(philo->data->death);
		// Check if philosopher has eaten enough meals
		if ((philo->data->eat_counter != -1) && 
		    (philo->eat_counter >= philo->data->max_eat) &&
		    (philo->data->max_eat > 0))
		{
			print_status(STATE_DONE, philo);
			philo->is_dead = TRUE;
			sem_post(philo->data->stop);
			sem_post(philo->data->death);
			break;
		}
		sem_post(philo->data->death);
		
		// Small sleep to avoid CPU hogging
		usleep(500);
	}
	return (NULL);
}

/**
 * Main routine for each philosopher
 * 
 * @param philo Pointer to philosopher structure
 */
void	run_philosopher_lifecycle(t_philo *philo)
{
	pthread_t	health_monitor;

	// Initialize next meal deadline
	philo->next_meal = get_timestamp_ms() + (unsigned int)philo->data->time_to_die;
	
	// Create health monitoring thread
	if (pthread_create(&health_monitor, NULL, monitor_philosopher_health, philo) != 0)
	{
		error_exit(philo->data, "Failed to create monitoring thread", 1);
		return;
	}
	pthread_detach(health_monitor);
	
	// Even-numbered philosophers start with a small delay to reduce deadlocks
	if (philo->id % 2 == 0)
		usleep(1000);
		
	// Main philosopher lifecycle loop
	while (!philo->is_dead)
	{
		philosopher_take_forks(philo);
		philosopher_eat(philo);
		philosopher_sleep(philo);
		print_status(STATE_THINKING, philo);
	}
}