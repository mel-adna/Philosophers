#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

# define SEM_FORKS "/forks"
# define SEM_PRINT "/print_lock"
# define SEM_STOP "/stop_sim"
# define SEM_MEAL "/meal_gate"

typedef struct s_args
{
	int		n_philo;
	long	t_die;
	long	t_eat;
	long	t_sleep;
	int		must_eat;
	long	start_time;
	sem_t	*forks;
	sem_t	*print_lock;
	sem_t	*stop_sim;
	sem_t	*meal_gate;
	sem_t	*death;
	pid_t	*pids;
}			t_args;

typedef struct s_philo
{
	int		id;
	int		eat_count;
	long	last_meal;
	t_args	*args;
}			t_philo;

long		get_time_ms(void);
void		my_usleep(long ms);
int			parse_args(int argc, char **argv, t_args *args);
void		print_status(t_args *args, int id, const char *msg);
int			ft_atoi(const char *str);

#endif
