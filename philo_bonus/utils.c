/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-adna <mel-adna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/30 18:11:11 by mbari             #+#    #+#             */
/*   Updated: 2025/05/21 10:13:30 by mel-adna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * Get current timestamp in milliseconds
 * 
 * @return Current timestamp in milliseconds
 */
unsigned int	get_timestamp_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

/**
 * Sleep for specified milliseconds with improved accuracy
 * 
 * @param ms Time to sleep in milliseconds
 */
void	sleep_ms(unsigned int ms)
{
	unsigned int	start;
	unsigned int	time_elapsed;
	
	start = get_timestamp_ms();
	while (1)
	{
		time_elapsed = get_timestamp_ms() - start;
		if (time_elapsed >= ms)
			break;
		usleep(500);
	}
}

/**
 * Convert string to integer
 * 
 * @param str String to convert
 * @return Integer value
 */
int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || 
			str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

/**
 * Compare two strings
 * 
 * @param s1 First string
 * @param s2 Second string
 * @return 0 if strings are equal, difference between first differing characters otherwise
 */
int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

/**
 * Display error message and exit with specified return code
 * 
 * @param simulation Pointer to simulation structure (can be NULL)
 * @param message Error message to display
 * @param ret Return code
 * @return Return code (for convenience in return statements)
 */
int	error_exit(t_simulation *simulation, char *message, int ret)
{
	(void)simulation;
	
	if (message)
		printf("Error: %s\n", message);
	
	return (ret);
}