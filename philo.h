#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define FINISHED 3

typedef struct s_data	t_data;

typedef struct s_philo
{
	t_data				*data;
	pthread_t			t1;
	pthread_t			supervisor;
	int					id;
	int					eat_count;
	int					status;
	int					eating;
	uint64_t			time_to_die;
	pthread_mutex_t		lock;
	pthread_mutex_t		*r_fork;
	pthread_mutex_t		*l_fork;
}						t_philo;

typedef struct s_data
{
	pthread_t			*tid;
	int					philo_num;
	int					meals_nb;
	int					dead;
	int					finished;
	t_philo				*philos;
	uint64_t			death_time;
	uint64_t			eat_time;
	uint64_t			sleep_time;
	uint64_t			start_time;
	pthread_mutex_t		*forks;
	pthread_mutex_t		lock;
	pthread_mutex_t		write;
}						t_data;

//======= Function prototypes ========

int						ft_atoi(const char *str);
uint64_t				get_time(void);
void					my_usleep(t_data *data, uint64_t time);
void					print_status(t_philo *philo, char *msg);
int						init_mutexes(t_data *data);
int						init_philos(t_data *data);
int						start_threads(t_data *data);
void					*routine(void *arg);
void					*supervisor(void *arg);
void					*monitor(void *arg);
void					start_eating(t_philo *philo);
int						check_input(int ac, char **av);
void					parse_args(int ac, char **av, t_data *data);
int						is_dead(t_data *data);
int						is_full(t_philo *philo);
int						wait_threads(t_data *data);
void					free_resources(t_data *data);

#endif
