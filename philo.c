#include "philo.h"

static void print_status(t_philo *philo, char *status)
{
    pthread_mutex_lock(&philo->data->write);
    if (!philo->data->dead)
        printf("%llu %d %s\n", (unsigned long long)(get_time() - philo->data->start_time), philo->id, status);
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
    pthread_mutex_unlock(&philo->lock);
    
    print_status(philo, "is eating");
    ft_usleep(philo->data->eat_time);
    
    pthread_mutex_lock(&philo->lock);
    philo->eat_cont++;
    philo->eating = 0;
    pthread_mutex_unlock(&philo->lock);

    // Release forks
    pthread_mutex_unlock(philo->r_fork);
    pthread_mutex_unlock(philo->l_fork);
}

static void *supervisor(void *arg)
{
    t_philo *philo;
    uint64_t current_time;

    philo = (t_philo *)arg;
    while (1)
    {
        // First check if the simulation is already over
        if (check_death(philo->data))
            break;
            
        current_time = get_time();
        pthread_mutex_lock(&philo->lock);
        
        // Check if philosopher is starving
        if (!philo->eating && current_time >= philo->time_to_die)
        {
            pthread_mutex_lock(&philo->data->lock);
            if (!philo->data->dead) // Only the first death matters
            {
                philo->data->dead = 1;
                pthread_mutex_unlock(&philo->data->lock);
                
                pthread_mutex_lock(&philo->data->write);
                printf("%llu %d died\n", (unsigned long long)(current_time - philo->data->start_time), philo->id);
                pthread_mutex_unlock(&philo->data->write);
            }
            else
            {
                pthread_mutex_unlock(&philo->data->lock);
            }
            
            pthread_mutex_unlock(&philo->lock);
            return (NULL);
        }
        
        // Check meal count
        if (philo->data->meals_nb != -1 && philo->eat_cont >= philo->data->meals_nb && !philo->status)
        {
            pthread_mutex_lock(&philo->data->lock);
            philo->status = 1;
            philo->data->finished++;
            pthread_mutex_unlock(&philo->data->lock);
        }
        
        pthread_mutex_unlock(&philo->lock);
        usleep(500); // More frequent checks on macOS
    }
    return (NULL);
}

void *philo_routine(void *arg)
{
    t_philo *philo;
    pthread_t tid;

    philo = (t_philo *)arg;
    pthread_mutex_lock(&philo->lock);
    philo->time_to_die = get_time() + philo->data->death_time;
    pthread_mutex_unlock(&philo->lock);
    
    if (pthread_create(&tid, NULL, &supervisor, philo))
        return (NULL);
    pthread_detach(tid);
    
    // If only one philosopher, handle special case
    if (philo->data->philo_num == 1)
    {
        print_status(philo, "has taken a fork");
        // Instead of sleeping, let the supervisor detect the death
        while (!check_death(philo->data))
            usleep(1000);
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