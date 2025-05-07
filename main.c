#include "philo.h"

static int is_numeric(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    return (1);
}

static int parse_args(int argc, char **argv, t_data *data) {
    int i;

    // Check that all arguments are numeric
    for (i = 1; i < argc; i++)
        if (!is_numeric(argv[i]))
            return error("Arguments must be numeric", NULL);

    data->philo_num = atoi(argv[1]);
    data->death_time = (uint64_t)atoi(argv[2]);
    data->eat_time = (uint64_t)atoi(argv[3]);
    data->sleep_time = (uint64_t)atoi(argv[4]);
    data->meals_nb = (argc == 6) ? atoi(argv[5]) : -1;
    
    // Validate ranges
    if (data->philo_num < 1 || data->philo_num > 200 || data->death_time <= 0 ||
        data->eat_time <= 0 || data->sleep_time <= 0 || (argc == 6 && data->meals_nb <= 0))
        return error("Invalid arguments", data);
    
    data->dead = 0;
    data->finished = 0;
    return 0;
}

int init_philos(t_data *data) {
    int i;

    // Initialize philos
    for (i = 0; i < data->philo_num; i++) {
        data->philos[i].id = i + 1;
        data->philos[i].data = data;
        data->philos[i].eat_cont = 0;
        data->philos[i].status = 0;
        data->philos[i].eating = 0;
        data->philos[i].time_to_die = 0;
        data->philos[i].r_fork = &data->forks[i];
        data->philos[i].l_fork = &data->forks[(i + 1) % data->philo_num];
        if (pthread_mutex_init(&data->philos[i].lock, NULL))
            return error("Mutex init failed (philo lock)", data);
    }
    return 0;
}

int create_threads(t_data *data) {
    int i;
    pthread_t monitor_tid;

    // Allocate thread IDs array
    data->tid = malloc(sizeof(pthread_t) * data->philo_num);
    if (!data->tid)
        return error("Malloc failed (tid)", data);

    // Record start time before launching threads
    data->start_time = get_time();
    
    // Create monitor thread if meals_nb is specified
    if (data->meals_nb != -1) {
        if (pthread_create(&monitor_tid, NULL, &monitor_routine, data))
            return error("Thread creation failed (monitor)", data);
        pthread_detach(monitor_tid);
    }

    // Create philosopher threads
    for (i = 0; i < data->philo_num; i++) {
        if (pthread_create(&data->tid[i], NULL, &philo_routine, &data->philos[i]))
            return error("Thread creation failed (philosopher)", data);
    }

    // Join all philosopher threads
    for (i = 0; i < data->philo_num; i++) {
        if (pthread_join(data->tid[i], NULL))
            return error("Thread join failed", data);
    }

    return 0;
}

int main(int argc, char **argv) {
    t_data data = {0};
    int i;

    if (argc < 5 || argc > 6)
        return error("Usage: ./philo num_philos time_to_die time_to_eat time_to_sleep [num_times_each_must_eat]", NULL);
    
    // Parse arguments
    if (parse_args(argc, argv, &data))
        return 1;
    
    // Allocate memory for forks and philosophers
    data.forks = malloc(sizeof(pthread_mutex_t) * data.philo_num);
    if (!data.forks)
        return error("Malloc failed (forks)", &data);
    
    data.philos = malloc(sizeof(t_philo) * data.philo_num);
    if (!data.philos)
        return error("Malloc failed (philos)", &data);
    
    // Initialize mutexes
    for (i = 0; i < data.philo_num; i++)
        if (pthread_mutex_init(&data.forks[i], NULL))
            return error("Mutex init failed (forks)", &data);
    
    if (pthread_mutex_init(&data.write, NULL) || pthread_mutex_init(&data.lock, NULL))
        return error("Mutex init failed (write/lock)", &data);
    
    // Initialize philosophers
    if (init_philos(&data))
        return 1;
    
    // Create and join threads
    if (create_threads(&data))
        return 1;
    
    // Clean up and exit
    ft_exit(&data);
    return 0;
}