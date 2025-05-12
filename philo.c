#include "philo.h"

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->r_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->l_fork);
	}
	else
	{
		pthread_mutex_lock(philo->l_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->r_fork);
	}
	print_status(philo, "has taken a fork");
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->lock);
	print_status(philo, "is eating");
	philo->time_to_die = get_time();
	philo->eating = 1;
	philo->eat_count++;
	pthread_mutex_unlock(&philo->lock);
	my_usleep(philo->data->eat_time);
	pthread_mutex_lock(&philo->lock);
	if (philo->data->meals_nb != -1
		&& philo->eat_count >= philo->data->meals_nb)
		philo->is_full = 1;
	philo->eating = 0;
	pthread_mutex_unlock(&philo->lock);
}

void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

int	check_meals(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->lock);
	if (philo->is_full)
		philo->data->finished++;
	if (philo->data->finished >= philo->data->philo_num)
	{
		philo->data->dead = 1;
		pthread_mutex_unlock(&philo->data->lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->lock);
	return (0);
}

void	check_if_all_full(t_data *data)
{
	pthread_mutex_lock(&data->lock);
	if (data->meals_nb != -1 && data->finished >= data->philo_num)
		data->dead = 1;
	pthread_mutex_unlock(&data->lock);
}

void	*routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (!data->dead)
	{
		take_forks(philo);
		eat(philo);
		release_forks(philo);
		if (check_meals(philo))
			break ;
		print_status(philo, "is sleeping");
		my_usleep(data->sleep_time);
		print_status(philo, "is thinking");
	}
	return (NULL);
}

void	*monitoring(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (!data->dead)
	{
		i = 0;
		while (i < data->philo_num)
		{
			pthread_mutex_lock(&data->philos[i].lock);
			if (get_time() - data->philos[i].time_to_die > data->death_time
				&& !data->philos[i].eating)
			{
				print_status(&data->philos[i], "died");
				data->dead = 1;
				pthread_mutex_unlock(&data->philos[i].lock);
				return (NULL);
			}
			pthread_mutex_unlock(&data->philos[i].lock);
			check_if_all_full(data);
			i++;
		}
		usleep(100);
	}
	return (NULL);
}
void	*single_philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	pthread_mutex_lock(philo->l_fork);
	print_status(philo, "has taken a fork");
	while (!data->dead)
		usleep(100);
	pthread_mutex_unlock(philo->l_fork);
	return (NULL);
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i].lock);
		i++;
	}
	pthread_mutex_destroy(&data->lock);
	pthread_mutex_destroy(&data->write);
	free(data->forks);
	free(data->philos);
}
int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	mon;
	int			i;

	if (argc < 5 || argc > 6)
		return (printf("Usage: ./philo num t_die t_eat t_sleep [num_eat]"));
	if (parse_args(argc, argv, &data) || init_mutex(&data)
		|| init_philos(&data))
		return (1);
	data.start_time = get_time();
	i = -1;
	while (++i < data.philo_num)
	{
		data.philos[i].time_to_die = data.start_time;
		if (data.philo_num == 1)
			pthread_create(&data.philos[i].t1, NULL, &single_philo_routine,
				&data.philos[i]);
		else
			pthread_create(&data.philos[i].t1, NULL, &routine, &data.philos[i]);
	}
	if (pthread_create(&mon, NULL, &monitoring, &data))
		return (1);
	pthread_join(mon, NULL);
	i = -1;
	while (++i < data.philo_num)
		pthread_join(data.philos[i].t1, NULL);
	cleanup(&data);
	return (0);
}
