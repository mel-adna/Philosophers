#include "philo.h"

static void print_status(t_philo *philo, char *status)
{
    pthread_mutex_lock(&philo->data->write);
    if (!philo->data->dead)
        printf("%lu %d %s\n", get_time() - philo->data->start_time, philo->id, status);
    pthread_mutex_unlock(&philo->data->write);
}

static void philo_eat(t_philo *philo)
{
    // Take forks
    pthread_mutex_lock(philo->r_fork);
    print_status(philo, "has taken a fork");
    pthread_mutex_lock(philo->l_fork);
    print_status(philo, "has taken a fork");

    // Eating
    pthread_mutex_lock(&philo->lock);
    philo->eating = 1;
    philo->time_to_die = get_time() + philo->data->death_time;
    print_status(philo, "is eating");
    philo->eat_cont++;
    ft_usleep(philo->data->eat_time);
    philo->eating = 0;
    pthread_mutex_unlock(&philo->lock);

    // Release forks
    pthread_mutex_unlock(philo->r_fork);
    pthread_mutex_unlock(philo->l_fork);
}

static void *supervisor(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    while (!check_death(philo->data))
    {
        pthread_mutex_lock(&philo->lock);
        if (!philo->eating && get_time() > philo->time_to_die)
        {
            print_status(philo, "died");
            pthread_mutex_lock(&philo->data->lock);
            philo->data->dead = 1;
            pthread_mutex_unlock(&philo->data->lock);
            pthread_mutex_unlock(&philo->lock);
            break;
        }
        if (philo->eat_cont >= philo->data->meals_nb && philo->data->meals_nb != -1 && !philo->status)
        {
            pthread_mutex_lock(&philo->data->lock);
            philo->status = 1;
            philo->data->finished++;
            pthread_mutex_unlock(&philo->data->lock);
            pthread_mutex_unlock(&philo->lock);
            break;
        }
        pthread_mutex_unlock(&philo->lock);
        ft_usleep(1);
    }
    return (NULL);
}

void *philo_routine(void *arg)
{
    t_philo *philo;
    pthread_t tid;

    philo = (t_philo *)arg;
    philo->time_to_die = get_time() + philo->data->death_time;
    if (pthread_create(&tid, NULL, &supervisor, philo))
        return (NULL);
    pthread_detach(tid);
    
    // If only one philosopher, handle special case
    if (philo->data->philo_num == 1)
    {
        print_status(philo, "has taken a fork");
        ft_usleep(philo->data->death_time);
        return (NULL);
    }

    // Stagger the philosophers to avoid deadlocks
    if (philo->id % 2 == 0)
        ft_usleep(10);

    while (!check_death(philo->data) && 
          (philo->data->meals_nb == -1 || philo->eat_cont < philo->data->meals_nb))
    {
        philo_eat(philo);
        print_status(philo, "is sleeping");
        ft_usleep(philo->data->sleep_time);
        print_status(philo, "is thinking");
    }
    return (NULL);
}