#include "philo_bonus.h"

uint64_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((uint64_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

void	my_usleep(uint64_t time_in_ms, t_philo *philo)
{
	uint64_t	start;
	uint64_t	end;

	(void)philo;
	start = get_time();
	while (get_time() - start < time_in_ms)
	{
		end = get_time() - start;
		if (end >= time_in_ms)
			break ;
		usleep(500);
	}
		
}

void	print_status(t_philo *philo, char *status)
{
	t_args	*args;

	args = philo->args;
	sem_wait(args->print_lock);
	printf("%lld %d %s\n", (get_time() - args->start_time),
		philo->id, status);
	sem_post(args->print_lock);
}

int	ft_atoi(const char *str)
{
	int				i;
	int				sign;
	unsigned long	result;

	i = 0;
	sign = 1;
	result = 0;
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			sign = -1;
	while (str[i] >= '0' && str[i] <= '9')
		result = result * 10 + (str[i++] - '0');
	if (result > 2147483647)
		return (-1);
	return ((int)(result * sign));
}
