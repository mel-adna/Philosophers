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
	static int	i = 0;

	pthread_mutex_lock(&philo->data->write); 
	if (!philo->data->dead)
	{
		printf("%llu %d %s\n", get_time() - philo->data->start_time, philo->id,
			msg);
	}
	else if (i != 1)
	{
		i++;
		printf("%llu %d %s\n", get_time() - philo->data->start_time, philo->id,
			"is died");
	}
	pthread_mutex_unlock(&philo->data->write);
}

// void	print_status(t_philo *philo, char *msg)
// {
// 	uint64_t	timestamp;

// 	timestamp = get_time() - philo->data->start_time;
// 	pthread_mutex_lock(&philo->data->write);
// 	if (!is_dead(philo->data))
// 		printf("%llu %d %s\n", timestamp, philo->id, msg);
// 	pthread_mutex_unlock(&philo->data->write);
// }


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
