/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-adna <mel-adna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/30 18:11:11 by mbari             #+#    #+#             */
/*   Updated: 2025/05/21 10:19:21 by mel-adna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * Main function - entry point of the program
 * 
 * @param ac Argument count
 * @param av Argument values
 * @return 0 on success, 1 on failure
 */
int	main(int ac, char **av)
{
	t_simulation	simulation;
	t_philo			*philo;

	if (ac == 5 || ac == 6)
	{
		// Initialize simulation structure to zeros
		memset(&simulation, 0, sizeof(t_simulation));
		
		// Parse command line arguments
		if (parse_arguments(av, &simulation))
			return (error_exit(&simulation, "Invalid arguments", 1));
		
		// Initialize philosopher structures	
		philo = init_philosophers(&simulation);
		if (!philo)
			return (error_exit(&simulation, "Failed to initialize philosophers", 1));
		
		// Record simulation start time	
		simulation.start = get_timestamp_ms();
		
		// Initialize semaphores
		if (init_semaphores(&simulation))
			return (error_exit(&simulation, "Failed to initialize semaphores", 1));
		
		// Create processes for philosophers
		create_philosopher_processes(&simulation, philo);
		
		// Wait for simulation to end (signaled by stop semaphore)
		sem_wait(simulation.stop);
		
		// Clean up all resources
		cleanup_resources(&simulation, philo);
	}
	else
		return (error_exit(NULL, "Invalid number of arguments", 1));
		
	return (0);
}
