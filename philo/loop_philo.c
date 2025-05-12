/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-adna <mel-adna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 14:42:59 by slamhaou          #+#    #+#             */
/*   Updated: 2025/05/03 19:37:42 by mel-adna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	did_philo_die(t_inform *p)
{
	pthread_mutex_lock(p->can_use_data);
	if (p->is_liv[0] == 0)
	{
		pthread_mutex_unlock(p->can_use_data);
		return (1);
	}
	pthread_mutex_unlock(p->can_use_data);
	return (0);
}

void	start_eat(t_inform *p)
{
	pthread_mutex_lock(p->right_fork);
	print_action(" has taken a fork", p, 'p');
	pthread_mutex_lock(p->left_fork);
	print_action(" has taken a fork", p, 'p');
	if (did_philo_die(p))
	{
		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);
		return ;
	}
	print_action(" is eating", p, 'p');
	my_usleeeep(p->tim.eat);
	pthread_mutex_lock(p->can_use_data);
	p->tim.last_tim_et = get_tim_now(p->tim.tim_start);
	if (p->tim.max_mell > 0)
		p->tim.max_mell--;
	pthread_mutex_unlock(p->can_use_data);
	pthread_mutex_unlock(p->right_fork);
	pthread_mutex_unlock(p->left_fork);
	if (did_philo_die(p))
		return ;
	print_action(" is sleeping", p, 'p');
	my_usleeeep(p->tim.slep);
	print_action("  is thinking", p, 'p');
}

void	odd_id(t_inform *p)
{
	if (p->id % 2 != 0)
	{
		print_action(" is sleeping ", p, 'p');
		my_usleeeep(p->tim.slep);
	}
}

void	*loop_philo(void *arg)
{
	t_inform	*p;

	p = (t_inform *)arg;
	odd_id(p);
	if (!p->left_fork)
	{
		pthread_mutex_lock(p->right_fork);
		print_action(" has taken a fork", p, 'p');
		return (NULL);
	}
	while (1)
	{
		if (did_philo_die(p) || p->tim.max_mell == 0)
			break ;
		start_eat(p);
	}
	return (NULL);
}
