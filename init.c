#include "philo.h"

int	init_mutex(t_data *data)
{
    int	i;

    data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_num);
    if (!data->forks)
        return (1);
    i = 0;
    while (i < data->philo_num)
    {
        if (pthread_mutex_init(&data->forks[i], NULL))
            return (1);
        i++;
    }
    if (pthread_mutex_init(&data->lock, NULL))
        return (1);
    if (pthread_mutex_init(&data->write, NULL))
        return (1);
    return (0);
}

int	init_philos(t_data *data)
{
    int	i;

    data->philos = malloc(sizeof(t_philo) * data->philo_num);
    if (!data->philos)
        return (1);
    i = 0;
    while (i < data->philo_num)
    {
        data->philos[i].id = i + 1;
        data->philos[i].eat_count = 0;
        data->philos[i].is_full = 0;
        data->philos[i].eating = 0;
        data->philos[i].time_to_die = get_time();
        data->philos[i].data = data;
        data->philos[i].r_fork = &data->forks[i];
        data->philos[i].l_fork = (i == 0) ? 
            &data->forks[data->philo_num - 1] : &data->forks[i - 1];
        if (pthread_mutex_init(&data->philos[i].lock, NULL))
            return (1);
        i++;
    }
    return (0);
}
