#include "philo_bonus.h"

void	*monitor_death(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if ((get_time_ms() - philo->last_meal) >= philo->args->t_die)
		{
			sem_wait(philo->args->print_lock);
			printf("%ld %d %s\n", get_time_ms() - philo->args->start_time,
				philo->id + 1, "died");
			sem_post(philo->args->stop_sim);
			exit(1);
		}
		usleep(100);
	}
	return (NULL);
}

void	philo_eat_cycle(t_philo *philo)
{
	t_args	*args;

	args = philo->args;
	sem_wait(args->meal_gate);
	sem_wait(args->forks);
	print_status(args, philo->id, "has taken a fork");
	sem_wait(args->forks);
	print_status(args, philo->id, "has taken a fork");
	print_status(args, philo->id, "is eating");
	philo->last_meal = get_time_ms();
	my_usleep(args->t_eat);
	philo->eat_count++;
	sem_post(args->forks);
	sem_post(args->forks);
	sem_post(args->meal_gate);
}

void	philo_routine(t_args *args, int id)
{
	t_philo		*philo;
	pthread_t	monitor;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		exit(1);
	philo->args = args;
	philo->id = id;
	philo->eat_count = 0;
	philo->last_meal = get_time_ms();
	pthread_create(&monitor, NULL, monitor_death, philo);
	pthread_detach(monitor);
	while (1)
	{
		print_status(args, id, "is thinking");
		philo_eat_cycle(philo);
		if (args->must_eat > 0 && philo->eat_count >= args->must_eat)
			{
				free(philo);
				exit(0);
			}
		print_status(args, id, "is sleeping");
		my_usleep(args->t_sleep);
	}
	usleep(10);
}

int	launch_philosophers(t_args *args)
{
	int	i;

	args->start_time = get_time_ms();
	i = 0;
	while (i < args->n_philo)
	{
		args->pids[i] = fork();
		if (args->pids[i] < 0)
			return (printf("Fork failed\n"), 1);
		if (args->pids[i] == 0)
		{
			philo_routine(args, i);
			exit(0);
		}
		i++;
	}
	return (0);
}

void	wait_for_philos(t_args *args)
{
	int	i;
	int	status;

	i = 0;
	while (i < args->n_philo)
	{
		waitpid(args->pids[i], &status, 0);
		i++;
	}
	sem_post(args->stop_sim);
}

void	cleanup(t_args *args)
{
	if (args->pids)
		free(args->pids);
	sem_close(args->forks);
	sem_close(args->print_lock);
	sem_close(args->stop_sim);
	sem_close(args->meal_gate);
	sem_unlink("/forks");
	sem_unlink("/print_lock");
	sem_unlink("/stop_sim");
	sem_unlink("/meal_gate");
}

void	*simulation_monitor(void *arg)
{
	int		i;
	t_args	*args;

	i = 0;
	args = (t_args *)arg;
	sem_wait(args->stop_sim);
	while (i < args->n_philo)
	{
		if (args->pids[i] > 0)
			kill(args->pids[i], SIGTERM);
		i++;
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_args		args;
	pthread_t	monitor_thread;

	if (argc < 5 || argc > 6)
		return (printf("Error: Wrong number of arguments\n"));
	if (parse_args(argc, argv, &args) || launch_philosophers(&args))
		return (1);
	pthread_create(&monitor_thread, NULL, simulation_monitor, &args);
	wait_for_philos(&args);
	pthread_join(monitor_thread, NULL);
	cleanup(&args);
	return (0);
}
