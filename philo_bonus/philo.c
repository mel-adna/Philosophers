#include "philo_bonus.h"

void	eating(t_philo *philo)
{
	t_args	*args;

	args = philo->args;
	sem_wait(args->forks);
	print_status(philo, "has taken a fork");
	sem_wait(args->forks);
	print_status(philo, "has taken a fork");
	sem_wait(args->lock);
	philo->last_meal = get_time();
	sem_post(args->lock);
	print_status(philo, "is eating");
	my_usleep(args->t_eat, philo);
	sem_post(args->forks);
	sem_post(args->forks);
	if (args->must_eat != -1)
	{
		sem_wait(args->lock);
		philo->eat_count++;
		if (philo->eat_count == args->must_eat)
			sem_post(args->meal_gate);
		sem_post(args->lock);
	}
}

void	*check_death(void *arg)
{
	t_philo	*philo;
	t_args	*args;

	philo = (t_philo *)arg;
	args = philo->args;
	while (1)
	{
		sem_wait(args->lock);
		if (get_time() - philo->last_meal >= args->t_die)
		{
			sem_wait(args->print_lock);
			printf("%lld %d died\n", (get_time() - args->start_time),
				philo->id);
			sem_post(args->stop_sim);
			exit(1);
		}
		sem_post(args->lock);
		usleep(100);
	}
	return (NULL);
}

void	*death_monitor(void *arg)
{
	t_args	*args;

	args = (t_args *)arg;
	sem_wait(args->stop_sim);
	exit(1);
	return (NULL);
}

void	philo_routine(t_philo *philo)
{
	t_args		*args;
	pthread_t	death_thread;
	pthread_t	monitor_thread;

	args = philo->args;
	pthread_create(&death_thread, NULL, &check_death, philo);
	pthread_detach(death_thread);
	pthread_create(&monitor_thread, NULL, &death_monitor, args);
	pthread_detach(monitor_thread);
	while (1)
	{
		print_status(philo, "is thinking");
		eating(philo);
		print_status(philo, "is sleeping");
		my_usleep(args->t_sleep, philo);
		if (args->must_eat != -1 && philo->eat_count > args->must_eat)
			exit(0);
	}
}

void	cleanning(t_args *args)
{
	sem_close(args->forks);
	sem_close(args->print_lock);
	sem_close(args->stop_sim);
	sem_close(args->meal_gate);
	sem_close(args->lock);
	sem_unlink("/forks");
	sem_unlink("/print_lock");
	sem_unlink("/stop_sim");
	sem_unlink("/meal_gate");
	sem_unlink("/lock");
	free(args->pids);
}

void	wait_children(t_args *args, pid_t *pids)
{
	int		i;
	int		status;
	pid_t	finished_pid;

	finished_pid = wait(&status);
	i = 0;
	while (i < args->n_philo)
	{
		kill(pids[i], SIGKILL);
		i++;
	}
	i = 0;
	while (i < args->n_philo)
		waitpid(pids[i++], NULL, 0);
}

void	philo_children(t_args *args, pid_t *pids, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < args->n_philo)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			philos[i].id = i + 1;
			philos[i].args = args;
			philos[i].last_meal = args->start_time;
			philos[i].eat_count = 0;
			philo_routine(&philos[i]);
		}
		else if (pids[i] < 0)
		{
			printf("Error: Fork failed\n");
			exit(1);
		}
		i++;
	}
}

void	start_simulation(t_args *args, t_philo *philos, pid_t *pids)
{
	args->start_time = get_time();
	if (args->n_philo == 1)
	{
		philos[0].id = 1;
		philos[0].args = args;
		philos[0].last_meal = args->start_time;
		philos[0].eat_count = 0;
		print_status(&philos[0], "has taken a fork");
		my_usleep(args->t_die, &philos[0]);
		print_status(&philos[0], "died");
		cleanning(args);
		exit(0);
	}
	philo_children(args, pids, philos);
	wait_children(args, pids);
	cleanning(args);
}

int	main(int argc, char **argv)
{
	t_args	args;
	t_philo	*philos;

	if (argc < 5 || argc > 6)
		return (printf("Error: Wrong number of arguments\n"));
	if (parse_args(argc, argv, &args) || init_sems(&args))
		return (1);
	philos = malloc(sizeof(t_philo) * args.n_philo);
	if (!philos)
		return (printf("Failed to allocate memory\n"));
	args.pids = malloc(sizeof(pid_t) * args.n_philo);
	if (!args.pids)
		return (free(philos), printf("Failed to allocate memory\n"));
	start_simulation(&args, philos, args.pids);
	return (0);
}
