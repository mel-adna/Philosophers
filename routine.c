#include "philo.h"

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (pthread_create(&philo->supervisor, NULL, &supervisor, philo) != 0)
		return (NULL);
	if (philo->id % 2 == 0)
		usleep(1000);
	while (!is_dead(philo->data) && !is_full(philo))
	{
		print_status(philo, "is thinking");
		pthread_mutex_lock(philo->l_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->r_fork);
		print_status(philo, "has taken a fork");
		start_eating(philo);
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		print_status(philo, "is sleeping");
		my_usleep(philo->data->sleep_time);
	}
	return (NULL);
}

void	start_eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->lock);
	philo->eating = 1;
	philo->time_to_die = get_time() + philo->data->death_time;
	print_status(philo, "is eating");
	pthread_mutex_unlock(&philo->lock);
	my_usleep(philo->data->eat_time);
	philo->eat_count++;
	if (philo->data->meals_nb > 0 && philo->eat_count >= philo->data->meals_nb)
		philo->status = FINISHED;
	pthread_mutex_lock(&philo->lock);
	philo->eating = 0;
	pthread_mutex_unlock(&philo->lock);
}
