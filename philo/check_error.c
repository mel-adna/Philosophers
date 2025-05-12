/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slamhaou <slamhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:05:01 by slamhaou          #+#    #+#             */
/*   Updated: 2025/05/01 16:33:33 by slamhaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_err(char *str, void *arr)
{
	int	i;

	i = 0;
	if (arr)
		free(arr);
	if (!str)
		return (1);
	while (str[i])
		write(1, &str[i++], 1);
	return (-5);
}

int	is_digets(char c)
{
	if (c >= '0' && c <= '9')
		return (0);
	return (1);
}

int	chake_str(char *str)
{
	int	i;

	i = 0;
	while (str[i] <= 32)
		i++;
	while (str[i])
	{
		if (is_digets(str[i]) && !(str[i] == '+' && is_digets(str[i + 1]) == 0))
			return (1);
		i++;
	}
	return (0);
}

int	chacke_args(char **av, int ac)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (av[i][0] == '\0')
			return (1);
		if (chake_str(av[i]))
			return (1);
		i++;
	}
	return (0);
}
