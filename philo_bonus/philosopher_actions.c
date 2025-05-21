/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_actions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-adna <mel-adna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/30 18:11:11 by mbari             #+#    #+#             */
/*   Updated: 2025/05/21 10:19:21 by mel-adna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * Initialize philosopher structures
 * 
 * @param simulation Pointer to simulation structure
 * @return Pointer to array of philosopher structures
 */
t_philo	*init_philosophers(t_simulation *simulation)
{
	t_philo	*philo;
	int		i;

	philo = malloc(sizeof(t_philo) * simulation->philo_numbers);
	if (!philo)
		return (NULL);
		
	memset(philo, 0, sizeof(t_philo) * simulation->philo_numbers);
	i = 0;
	while (i < simulation->philo_numbers)
	{
		philo[i].id = i + 1;
		philo[i].pid = 0;
		philo[i].next_meal = 0;
		philo[i].eating_time = 0;
		philo[i].is_dead = FALSE;
		philo[i].eat_counter = 0;
		philo[i].data = simulation;
		i++;
	}
	return (philo);
}

/**
 * Philosopher takes forks and prints status
 * 
 * @param philo Pointer to philosopher structure
 */
void	philosopher_take_forks(t_philo *philo)
{
	// Check if philosopher is already dead before attempting to take forks
	if (philo->is_dead)
		return;
		
	sem_wait(philo->data->forks);
	print_status(STATE_FORK, philo);
	
	// Check again after taking first fork
	if (philo->is_dead)
	{
		sem_post(philo->data->forks);
		return;
	}
	
	sem_wait(philo->data->forks);
	print_status(STATE_FORK, philo);
}

/**
 * Philosopher eats, updates meal time, and releases forks
 * 
 * @param philo Pointer to philosopher structure
 */
void	philosopher_eat(t_philo *philo)
{
	// Check if philosopher is already dead
	if (philo->is_dead)
	{
		sem_post(philo->data->forks);
		sem_post(philo->data->forks);
		return;
	}
	
	print_status(STATE_EATING, philo);
	
	sem_wait(philo->data->death);
	philo->next_meal = get_timestamp_ms() + (unsigned int)philo->data->time_to_die;
	philo->eating_time = get_timestamp_ms();
	
	if (philo->data->eat_counter != -1)
	{
		philo->data->current_eat++;
		philo->eat_counter++;
	}
	sem_post(philo->data->death);
	
	// Use our more precise sleep function
	sleep_ms((unsigned int)philo->data->time_to_eat);
	
	// Release the forks
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
}

/**
 * Philosopher sleeps and prints status
 * 
 * @param philo Pointer to philosopher structure
 */
void	philosopher_sleep(t_philo *philo)
{
	// Check if philosopher is already dead
	if (philo->is_dead)
		return;
		
	print_status(STATE_SLEEPING, philo);
	sleep_ms((unsigned int)philo->data->time_to_sleep);
}

/**
 * Print philosopher status with timestamp
 * 
 * @param status Status to print
 * @param philo Pointer to philosopher structure
 */
void	print_status(char *status, t_philo *philo)
{
	unsigned int	timestamp;

	// Don't print status updates if philosopher is already dead
	// Exception: allow printing death and done messages
	if (philo->is_dead && 
		ft_strcmp(status, STATE_DIED) != 0 && 
		ft_strcmp(status, STATE_DONE) != 0)
		return;

	sem_wait(philo->data->print);
	timestamp = get_timestamp_ms() - philo->data->start;
	printf("%u %d %s\n", timestamp, philo->id, status);
	
	// Only release the semaphore if it's not a terminal state message
	if (ft_strcmp(status, STATE_DIED) != 0 && ft_strcmp(status, STATE_DONE) != 0)
		sem_post(philo->data->print);
}