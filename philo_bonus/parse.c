#include "philo_bonus.h"

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[0] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	set_values(int argc, char **argv, t_args *args)
{
	args->n_philo = ft_atoi(argv[1]);
	args->t_die = (uint64_t)ft_atoi(argv[2]);
	args->t_eat = (uint64_t)ft_atoi(argv[3]);
	args->t_sleep = (uint64_t)ft_atoi(argv[4]);
	if (argc == 6)
		args->must_eat = ft_atoi(argv[5]);
	else
		args->must_eat = -1;
	if (args->n_philo < 1 || args->n_philo > 200 || args->t_die <= 0
		|| args->t_eat <= 0 || args->t_sleep <= 0 || (argc == 6
			&& args->must_eat <= 0))
		return (0);
	return (1);
}

static int	init_semaphores(t_args *args)
{
	sem_unlink("/forks");
	sem_unlink("/print_lock");
	sem_unlink("/stop_sim");
	sem_unlink("/death");
	sem_unlink(SEM_MEAL);
	args->meal_gate = sem_open(SEM_MEAL, O_CREAT | O_EXCL, 0644, 
			args->n_philo / 2);
	if (args->meal_gate == SEM_FAILED)
		return (0);
	args->death = sem_open("/death", O_CREAT | O_EXCL, 0644, 1);
	if (args->death == SEM_FAILED)
		return (1);
	args->forks = sem_open("/forks", O_CREAT | O_EXCL, 0644, args->n_philo);
	args->print_lock = sem_open("/print_lock", O_CREAT | O_EXCL, 0644, 1);
	args->stop_sim = sem_open("/stop_sim", O_CREAT | O_EXCL, 0644, 0);
	if (args->forks == SEM_FAILED || args->print_lock == SEM_FAILED
		|| args->stop_sim == SEM_FAILED)
		return (0);
	return (1);
}

int	parse_args(int argc, char **argv, t_args *args)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (!is_numeric(argv[i]))
			return (printf("Invalid arguments\n"), 1);
		i++;
	}
	if (!set_values(argc, argv, args))
		return (printf("Invalid arguments\n"), 1);
	if (!init_semaphores(args))
		return (printf("Semaphore error\n"), 1);
	args->pids = malloc(sizeof(pid_t) * args->n_philo);
	if (!args->pids)
		return (printf("Malloc error\n"), 1);
	args->start_time = get_time_ms();
	return (0);
}
