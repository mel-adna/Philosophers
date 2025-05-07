#include "philo.h"

int check_death(t_data *data)
{
    int result;
    
    pthread_mutex_lock(&data->lock);
    result = data->dead;
    pthread_mutex_unlock(&data->lock);
    return (result);
}

void *monitor_routine(void *arg)
{
    t_data *data;

    data = (t_data *)arg;
    while (1)
    {
        pthread_mutex_lock(&data->lock);
        if (data->dead)
        {
            pthread_mutex_unlock(&data->lock);
            break;
        }
        if (data->finished >= data->philo_num && data->meals_nb != -1)
        {
            data->dead = 1;
            pthread_mutex_unlock(&data->lock);
            break;
        }
        pthread_mutex_unlock(&data->lock);
        ft_usleep(1);
    }
    return (NULL);
}