#include "philo.h"

void *monitor(void *arg)
{
    t_data *data = (t_data *)arg;
    int i, full_count;
    uint64_t current_time;

    while (1)
    {
        i = 0;
        full_count = 0;
        while (i < data->philo_num)
        {
            pthread_mutex_lock(&data->philos[i].lock);
            current_time = get_time();
            // For debugging, uncomment if needed
            // debug_time_to_die(&data->philos[i]);
            if (current_time > data->philos[i].time_to_die)
            {
                pthread_mutex_lock(&data->write);
                pthread_mutex_lock(&data->lock);
                if (!data->dead) // Only print if not already marked as dead
                {
                    data->dead = 1;
                    printf("%lu %d died\n", current_time - data->start_time, data->philos[i].id);
                }
                pthread_mutex_unlock(&data->lock);
                pthread_mutex_unlock(&data->write);
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