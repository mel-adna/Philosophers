#include "philo.h"

uint64_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	ft_atoi(const char *str)
{
	int				i;
	int				sign;
	unsigned long	result;

	i = 0;
	result = 0;
	sign = 1;
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
		if (result > 2147483647 && sign == 1)
			return (-1);
		if (result > 2147483648 && sign == -1)
			return (0);
	}
	return ((int)(result * sign));
}

void	my_usleep(t_data *data, uint64_t time_in_ms)
{
	uint64_t	start;

	start = get_time();
	while ((get_time() - start) < time_in_ms && !is_dead(data))
		usleep(100);
}

void	print_status(t_philo *philo, char *msg)
{
	uint64_t	timestamp;

	timestamp = get_time() - philo->data->start_time;
	pthread_mutex_lock(&philo->data->write);
	if (!is_dead(philo->data))
		printf("%llu %d %s\n", timestamp, philo->id, msg);
	pthread_mutex_unlock(&philo->data->write);
}

int	is_dead(t_data *data)
{
	int	flag;
	pthread_mutex_lock(&data->lock);
	if (data->dead)
	{
		pthread_mutex_unlock(&data->lock);
		return (1);
	}
	flag = data->dead;
	pthread_mutex_unlock(&data->lock);
	return (flag);
}

int	is_full(t_philo *philo)
{
	if (philo->data->meals_nb > 0 && philo->eat_count >= philo->data->meals_nb)
	{
		philo->status = FINISHED;
		return (1);
	}
	return (0);
}

void *supervisor(void *arg)
{
    t_philo *philo;
    
    philo = (t_philo *)arg;
    while (!is_dead(philo->data))
    {
        pthread_mutex_lock(&philo->lock);
        if (!philo->eating && get_time() >= philo->time_to_die)
        {
            pthread_mutex_lock(&philo->data->lock);
            if (!philo->data->dead)
            {
                philo->data->dead = 1;
                pthread_mutex_unlock(&philo->data->lock);
                print_status(philo, "died");
                pthread_mutex_unlock(&philo->lock);
                return NULL;  // Exit immediately after death
            }
            pthread_mutex_unlock(&philo->data->lock);
        }
        pthread_mutex_unlock(&philo->lock);
        usleep(1000);  // Check more frequently - 1ms intervals
    }
    return NULL;
}

void start_eating(t_philo *philo)
{
    pthread_mutex_lock(&philo->lock);
    philo->eating = 1;
    // This is critical - set time_to_die correctly
    philo->time_to_die = get_time() + philo->data->death_time;
    print_status(philo, "is eating");
    pthread_mutex_unlock(&philo->lock);
    
    my_usleep(philo->data, philo->data->eat_time);
    philo->eat_count++;
    
    pthread_mutex_lock(&philo->lock);
    philo->eating = 0;
    pthread_mutex_unlock(&philo->lock);
    
    // Don't check meals_nb here - it's already handled in is_full
}
