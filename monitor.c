#include "philo.h"

void *monitor(void *arg)
{
    t_data *data = (t_data *)arg;
    int i, full_count;

    while (1)
    {
        i = 0;
        full_count = 0;
        while (i < data->philo_num)
        {
            pthread_mutex_lock(&data->philos[i].lock);
            if (!data->philos[i].eating && get_time() > data->philos[i].time_to_die)
            {
                pthread_mutex_lock(&data->lock);
                data->dead = 1;
                pthread_mutex_unlock(&data->lock);
                print_status(&data->philos[i], "died");
                pthread_mutex_unlock(&data->philos[i].lock);
                return NULL;
            }
            if (data->philos[i].is_full)
                full_count++;
            pthread_mutex_unlock(&data->philos[i].lock);
            i++;
        }
        pthread_mutex_lock(&data->lock);
        if (data->meals_nb != -1 && full_count == data->philo_num)
        {
            data->finished = 1;
            pthread_mutex_unlock(&data->lock);
            return NULL;
        }
        pthread_mutex_unlock(&data->lock);
        usleep(1000);
    }
}