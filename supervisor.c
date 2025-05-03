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
            pthread_mutex_lock(&philo->data->lock);
            if (!philo->data->dead)
            {
                philo->data->dead = 1;
                pthread_mutex_unlock(&philo->data->lock);
                print_status(philo, "died");
                pthread_mutex_unlock(&philo->lock);
                break;
            }
            pthread_mutex_unlock(&philo->data->lock);
        }
        pthread_mutex_unlock(&philo->lock);
        usleep(1000);
    }
    return (NULL);
}
