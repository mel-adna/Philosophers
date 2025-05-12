/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   organization_p_f.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slamhaou <slamhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 11:45:16 by slamhaou          #+#    #+#             */
/*   Updated: 2025/05/03 16:52:01 by slamhaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	waiting_thread(t_var *p)
{
	int	i;

	i = 0;
	while (i < p->n_philo)
	{
		pthread_join(p->info_phlo[i].thrd, NULL);
		i++;
	}
	i = 0;
	while (i < p->n_philo)
	{
		pthread_mutex_destroy(&p->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&p->use_data);
	free(p->forks);
	free(p->info_phlo);
}

void	philo_stract(t_var *p, int j)
{
	p->info_phlo[j].tim.die = p->tt.die;
	p->info_phlo[j].tim.eat = p->tt.eat;
	p->info_phlo[j].tim.slep = p->tt.slep;
	p->info_phlo[j].tim.last_tim_et = 0;
	p->info_phlo[j].tim.tim_start = p->tt.tim_start;
	p->info_phlo[j].can_use_data = &p->use_data;
	p->info_phlo[j].is_liv = &p->live;
	p->info_phlo[j].tim.max_mell = p->n_hw_mny_et;
}

void	organization_phlo_frk(t_var *p)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (j < p->n_philo)
	{
		p->info_phlo[j].id = i;
		if (j == 0)
		{
			p->info_phlo->right_fork = &p->forks[j];
			if (p->n_philo > 1)
				p->info_phlo->left_fork = &p->forks[p->n_philo - 1];
		}
		else
		{
			p->info_phlo[j].right_fork = &p->forks[j];
			p->info_phlo[j].left_fork = &p->forks[j - 1];
		}
		philo_stract(p, j);
		i++;
		j++;
	}
}

void	init_forks(t_var *p)
{
	int	i;

	i = 0;
	while (i < p->n_philo)
	{
		pthread_mutex_init(&p->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&p->use_data, NULL);
	p->live = 1;
}

void	print_action(char *str, t_inform *p, char c)
{
	long	tim;

	pthread_mutex_lock(p->can_use_data);
	tim = get_tim_now(p->tim.tim_start);
	if (p->is_liv[0] || (c == 'm' && p->is_liv[0] == 0))
		printf("%ld %d %s\n", tim, p->id, str);
	pthread_mutex_unlock(p->can_use_data);
}
