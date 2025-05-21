/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-adna <mel-adna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/30 18:11:38 by mbari             #+#    #+#             */
/*   Updated: 2025/05/21 10:19:21 by mel-adna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

/* Standard Library Includes */
# include <stdio.h>
# include <semaphore.h>
# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <signal.h>
# include <sys/wait.h>

/* Boolean Constants */
# define TRUE 1
# define FALSE 0

/* Philosopher States */
# define STATE_FORK "has taken a fork"
# define STATE_EATING "is eating"
# define STATE_SLEEPING "is sleeping"
# define STATE_THINKING "is thinking"
# define STATE_DIED "died"
# define STATE_DONE "has eaten enough"

/* Minimum time in milliseconds */
# define MIN_TIME_MS 60

/**
 * Structure to hold simulation settings and shared resources
 */
typedef struct s_simulation
{
	sem_t			*forks;          /* Semaphore for forks */
	sem_t			 *print;         /* Semaphore for console output */
	sem_t			*death;          /* Semaphore to check death status */
	sem_t			*stop;           /* Semaphore to stop simulation */
	unsigned int	start;           /* Simulation start timestamp in ms */
	int				philo_numbers;   /* Number of philosophers */
	int				time_to_die;     /* Time until philosopher dies without eating */
	int				time_to_eat;     /* Time it takes to eat */
	int				time_to_sleep;   /* Time it takes to sleep */
	int				eat_counter;     /* Required meals per philosopher (-1 for unlimited) */
	int				max_eat;         /* Required total meals for all philosophers */
	int				current_eat;     /* Current total meals eaten */
}				t_simulation;

/**
 * Structure to hold philosopher data
 */
typedef struct s_philo
{
	t_simulation	*data;           /* Pointer to simulation data */
	pid_t			pid;             /* Process ID for this philosopher */
	unsigned int	eating_time;     /* Time when last meal started */
	unsigned int	next_meal;       /* Time when philosopher will die */
	int				id;              /* Philosopher ID (1-indexed) */
	int				is_dead;         /* Philosopher death status */
	int				eat_counter;     /* Number of meals this philosopher has eaten */
}				t_philo;

/* Time functions */
unsigned int	get_timestamp_ms(void);
void			sleep_ms(unsigned int ms);

/* Core philosopher actions */
void			philosopher_routine(t_philo *philo);
void			philosopher_eat(t_philo *philo);
void			philosopher_sleep(t_philo *philo);
void			philosopher_take_forks(t_philo *philo);

/* Parsing functions */
int				parse_arguments(char **argv, t_simulation *simulation);
int				parse_positive_int(char *arg);
int				set_simulation_value(t_simulation *simulation, int value, int param_index);
int				set_optional_params(t_simulation *simulation, int value, int param_index);

/* Initialization functions */
t_philo			*init_philosophers(t_simulation *simulation);
int				init_semaphores(t_simulation *simulation);

/* Monitor functions */
void			*monitor_philosopher_health(void *arg);
void			print_status(char *status, t_philo *philo);

/* Process management */
void			create_philosopher_processes(t_simulation *simulation, t_philo *philo);
void			cleanup_resources(t_simulation *simulation, t_philo *philo);

/* Error handling */
int				error_exit(t_simulation *simulation, char *message, int ret);

/* main.c */
int				main(int ac, char **av);

/* utils.c */
unsigned int	get_timestamp_ms(void);
int				ft_atoi(const char *str);
int				ft_strcmp(const char *s1, const char *s2);

/* simulation.c */
int				parse_arguments(char **av, t_simulation *simulation);
int				init_semaphores(t_simulation *simulation);
void			create_philosopher_processes(t_simulation *simulation, t_philo *philo);

/* philosopher_actions.c */
t_philo			*init_philosophers(t_simulation *simulation);
void			philosopher_take_forks(t_philo *philo);
void			philosopher_eat(t_philo *philo);
void			philosopher_sleep(t_philo *philo);

/* philosopher_lifecycle.c */
void			run_philosopher_lifecycle(t_philo *philo);

#endif
