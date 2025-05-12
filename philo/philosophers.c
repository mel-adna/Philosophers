/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slamhaou <slamhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:25:37 by slamhaou          #+#    #+#             */
/*   Updated: 2025/05/03 17:39:53 by slamhaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	with_meal(t_var *p, pthread_mutex_t	*monit)
{
	if (p->n_hw_mny_et > 0)
	{
		if (all_f(p))
		{
			pthread_mutex_unlock(monit);
			return (1);
		}
	}
	return (0);
}

void	the_thrd_die(t_var *p)
{
	pthread_mutex_t	*monit;
	long			res;
	int				i;

	i = 0;
	monit = &p->use_data;
	while (1)
	{
		pthread_mutex_lock(monit);
		res = get_tim_now(p->tt.tim_start) - p->info_phlo[i].tim.last_tim_et;
		if (res >= p->tt.die && p->info_phlo[i].tim.max_mell != 0)
		{
			p->live = 0;
			pthread_mutex_unlock(monit);
			print_action("died", &p->info_phlo[i], 'm');
			break ;
		}
		else if (p->live && i == p->n_philo - 1)
			i = 0;
		else if (with_meal(p, monit))
			break ;
		else
			i++;
		pthread_mutex_unlock(monit);
	}
}

int	ft_creat_frk_trd(t_var *p)
{
	int	i;
	int	valid_thrd;

	i = 0;
	p->info_phlo = malloc(sizeof(t_inform) * p->n_philo);
	if (!p->info_phlo)
		return (ft_err("cant alloc", NULL), ERROR);
	p->forks = malloc(sizeof(pthread_mutex_t) * p->n_philo);
	if (!p->forks)
		return (ft_err("cant alloc", p->info_phlo), ERROR);
	organization_phlo_frk(p);
	init_forks(p);
	while (i < p->n_philo)
	{
		valid_thrd = pthread_create
			(&p->info_phlo[i].thrd, NULL, loop_philo, &p->info_phlo[i]);
		if (valid_thrd > 0)
			return (free(p->info_phlo), 
				ft_err("cant make thread!", p->forks), ERROR);
		i++;
	}
	the_thrd_die(p);
	waiting_thread(p);
	return (0);
}

int	philo(t_var *p, char **av)
{
	int	err;

	err = 1;
	p->n_philo = ft_atoi(av[1], &err);
	if (p->n_philo == 0)
		return (ERROR);
	p->tt.die = ft_atoi(av[2], &err);
	p->tt.eat = ft_atoi(av[3], &err);
	p->tt.slep = ft_atoi(av[4], &err);
	if (p->tt.eat == 0 || p->tt.slep == 0 
		|| p->tt.die == 0 || p->n_hw_mny_et == 0)
		return (ERROR);
	if (err == ERROR || ft_creat_frk_trd(p) == ERROR)
		return (ERROR);
	return (0);
}

int	main(int ac, char **av)
{
	t_var	p;

	p.tt.tim_start = get_tim_now(-1);
	if (!(ac == 5 || ac == 6))
		return (ft_err("Erorr input ", NULL), 1);
	else if (chacke_args(av, ac))
		return (ft_err("Error input ", NULL), 1);
	if (ac == 5)
		p.n_hw_mny_et = NO_MEAL;
	else if (ac == 6)
		p.n_hw_mny_et = ft_atoi(av[5], NULL);
	if (p.n_hw_mny_et == ERROR || philo(&p, av) == ERROR)
		return (ft_err("Erorr input", NULL), 1);
}
