/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_f.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slamhaou <slamhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 17:30:16 by slamhaou          #+#    #+#             */
/*   Updated: 2025/05/03 17:25:52 by slamhaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	all_f(t_var *p)
{
	int	i;
	int	count;

	i = 0;
	while (i < p->n_philo)
	{
		count = p->info_phlo[i].tim.max_mell;
		if (count != 0)
			return (0);
		i++;
	}
	return (1);
}

long	get_tim_now(long start)
{
	struct timeval	tim;
	long			t;

	gettimeofday(&tim, NULL);
	if (start == -1)
		return ((tim.tv_sec * 1000) + (tim.tv_usec / 1000));
	t = (tim.tv_sec * 1000) + (tim.tv_usec / 1000) - start;
	return (t);
}

void	my_usleeeep(long num_to_slep)
{
	long	i;

	i = get_tim_now(-1);
	while ((get_tim_now(-1) - i) < num_to_slep)
		usleep(50);
}

long	ft_atoi(char *str, int *err)
{
	long	res;
	int		i;

	i = 0;
	res = 0;
	while (str[i] <= 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			return (ERROR);
		i++;
	}
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - 48);
		if (res > INT_MAX)
		{
			if (err)
				*err = ERROR;
			return (ERROR);
		}
		i++;
	}
	return (res);
}
