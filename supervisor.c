#include "philo.h"

void	*supervisor(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!is_dead(philo->data))
	{
		pthread_mutex_lock(&philo->lock);
		if (!philo->eating && get_time() > philo->time_to_die)
		{
			print_status(philo, "died");
			philo->data->dead = 1;
			pthread_mutex_unlock(&philo->lock);
			break ;
		}
		pthread_mutex_unlock(&philo->lock);
		usleep(100);
	}
	return (NULL);
}
