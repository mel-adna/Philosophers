/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slamhaou <slamhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:25:56 by slamhaou          #+#    #+#             */
/*   Updated: 2025/05/03 17:36:36 by slamhaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
//--------------♥︎𝑰𝑵𝑪𝑳𝑼𝑫𝑬♥︎----------
//-------------===========-----------
# include <unistd.h>
# include <limits.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
//---------------♥︎𝑆𝑇𝑅𝑈𝐶𝑇♥︎-----------
//---------------========-------------
# define NO_MEAL -1
# define ERROR -5

typedef struct time
{
	long		die;
	long		eat;
	long		slep;
	long		max_mell;
	long		last_tim_et;
	long		tim_start;
}	t_time;

typedef struct philo_informations
{
	pthread_mutex_t			*right_fork;
	pthread_mutex_t			*left_fork;
	pthread_mutex_t			*can_use_data;
	pthread_t				thrd;
	t_time					tim;
	int						id;
	int						*is_liv;
}	t_inform;

typedef struct var_philo
{
	pthread_mutex_t	*forks;
	pthread_mutex_t	use_data;
	t_inform		*info_phlo;
	t_time			tt;
	int				n_philo;
	int				n_hw_mny_et;
	int				live;
}	t_var;
//----------------=====---------------------

int			chacke_args(char **av, int ac);
long		ft_atoi(char *str, int *err);
int			ft_err(char *str, void *arr);
void		organization_phlo_frk(t_var *p);
void		init_forks(t_var *p);
void		*loop_philo(void *arg);
void		print_action(char *str, t_inform *p, char c);
long		get_tim_now(long start);
void		my_usleeeep(long num_to_slep);
void		waiting_thread(t_var *p);
int			all_f(t_var *p);

#endif