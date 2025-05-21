/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-adna <mel-adna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/30 18:11:11 by mbari             #+#    #+#             */
/*   Updated: 2025/05/21 10:19:21 by mel-adna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * Creates and initializes all philosopher processes
 * 
 * @param simulation Pointer to simulation structure
 * @param philo Array of philosopher structures
 */
void	create_philosopher_processes(t_simulation *simulation, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < simulation->philo_numbers)
	{
		philo[i].pid = fork();
		if (philo[i].pid == 0)
		{
			run_philosopher_lifecycle(philo + i);
			exit(0);
		}
		i++;
		usleep(100);
	}
}

/**
 * Initialize semaphores used in the simulation
 * 
 * @param simulation Pointer to simulation structure
 * @return 0 on success, 1 on failure
 */
int	init_semaphores(t_simulation *simulation)
{
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/death");
	sem_unlink("/stop");
	
	simulation->forks = sem_open("/forks", O_CREAT, S_IRWXU, simulation->philo_numbers);
	simulation->print = sem_open("/print", O_CREAT, S_IRWXU, 1);
	simulation->death = sem_open("/death", O_CREAT, S_IRWXU, 1);
	simulation->stop = sem_open("/stop", O_CREAT, S_IRWXU, 0);
	
	if (simulation->forks == SEM_FAILED || simulation->print == SEM_FAILED ||
		simulation->death == SEM_FAILED || simulation->stop == SEM_FAILED)
		return (error_exit(simulation, "Failed to create semaphores", 1));
	
	return (0);
}

/**
 * Clean up resources used in the simulation
 * 
 * @param simulation Pointer to simulation structure
 * @param philo Array of philosopher structures
 */
void	cleanup_resources(t_simulation *simulation, t_philo *philo)
{
	int	i;
	int	status;

	// Kill all philosopher processes
	i = 0;
	while (i < simulation->philo_numbers)
	{
		if (philo[i].pid > 0)
			kill(philo[i].pid, SIGKILL);
		i++;
	}
	
	// Wait for all processes to terminate
	i = 0;
	while (i < simulation->philo_numbers)
	{
		waitpid(-1, &status, 0);
		i++;
	}
	
	// Close and unlink semaphores
	sem_close(simulation->forks);
	sem_close(simulation->print);
	sem_close(simulation->death);
	sem_close(simulation->stop);
	
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/death");
	sem_unlink("/stop");
	
	// Free allocated memory
	if (philo)
		free(philo);
}

/**
 * Parse command line arguments for the simulation
 * 
 * @param av Command line arguments
 * @param simulation Pointer to simulation structure
 * @return 0 on success, 1 on failure
 */
int	parse_arguments(char **av, t_simulation *simulation)
{
	simulation->philo_numbers = ft_atoi(av[1]);
	simulation->time_to_die = ft_atoi(av[2]);
	simulation->time_to_eat = ft_atoi(av[3]);
	simulation->time_to_sleep = ft_atoi(av[4]);
	simulation->max_eat = -1;
	simulation->current_eat = 0;
	simulation->eat_counter = -1;
	
	if (av[5])
	{
		simulation->max_eat = ft_atoi(av[5]);
		if (simulation->max_eat <= 0)
			return (1);
		simulation->eat_counter = 0;
	}
	
	if (simulation->philo_numbers <= 0 || simulation->time_to_die <= 0 ||
		simulation->time_to_eat <= 0 || simulation->time_to_sleep <= 0)
		return (1);
	
	return (0);
}