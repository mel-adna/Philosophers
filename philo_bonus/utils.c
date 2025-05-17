#include "philo_bonus.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	my_usleep(long ms)
{
	long	start;

	start = get_time_ms();
	while ((get_time_ms() - start) < ms)
		usleep(100);
}

void	print_status(t_args *args, int id, const char *msg)
{
	long	timestamp;

	sem_wait(args->print_lock);
	if (strcmp(msg, "died") != 0)
	{
		timestamp = get_time_ms() - args->start_time;
		printf("%ld %d %s\n", timestamp, id + 1, msg);
		sem_post(args->print_lock);
	}
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
