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

void	my_usleep(uint64_t time_in_ms)
{
	uint64_t	start;

	start = get_time();
	while ((get_time() - start) < time_in_ms)
		usleep(100);
}
void	print_status(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->data->write);
	if (!philo->data->dead)
	{
		printf("%llu %d %s\n", get_time() - philo->data->start_time, philo->id,
			msg);
	}
	pthread_mutex_unlock(&philo->data->write);
}

int	is_dead(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		if (data->philos[i].time_to_die <= get_time())
		{
			data->dead = 1;
			return (1);
		}
		i++;
	}
	return (0);
}

int	is_full(t_philo *philo)
{
	if (philo->data->meals_nb > 0 && philo->eat_count >= philo->data->meals_nb)
	{
		philo->status = FINISHED;
		return (1);
	}
	printf("Philo %d entered loop\n", philo->id);
	return (0);
}
