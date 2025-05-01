#include "philo.h"
#include <stdlib.h>
#include <time.h>

/*
long			x =  0;
pthread_mutex_t	mutex;
void	*routine(void *arg)
{
	(void)arg;
	for (int i = 0; i < 1000000 ; i++)
	{
		pthread_mutex_lock(&mutex);
		x ++;
		pthread_mutex_unlock(&mutex);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	pthread_t	th[11];
	int			i;

	(void)argc;
	(void)argv;
	pthread_mutex_init(&mutex, NULL);
	i = 0;
	while (i < 11)
	{
		if (pthread_create(&th[i], NULL, &routine, NULL) != 0)
			return (1);
		printf("thread %d has started\n", i);
		if (pthread_join(th[i], NULL) != 0)
			return (3);
		i ++;
		printf("thread %d has finished execution\n", i);
	}
	pthread_mutex_destroy(&mutex);
	printf("value of x: %ld\n", x);
	return (0);
}
=============================================

void	*roll_dice(void *arg)
{
	int	*res;

	(void)arg;
	res = malloc(sizeof(int));
	if (!res)
	{
		perror("malloc failed");
		return (NULL);
	}
	*res = (rand() % 6) + 1;
	return (res);
}

int	main(void)
{
	pthread_t	th[4];
	int			*res;

	srand(time(NULL));
	for (int i = 0; i < 4; i++)
	{
		if (pthread_create(&th[i], NULL, &roll_dice, NULL) != 0)
		{
			perror("Failed to create thread");
			return (1);
		}
		if (pthread_join(th[i], (void **)&res) != 0)
		{
			perror("Failed to join thread");
			return (2);
		}
		printf("Thread %d result: %p\n", i, (void *)res);
		printf("Dice value: %d\n", *res);
		free(res);
	}
	return (0);
}
==================================

int				primes[10] = {2, 3, 4, 5, 7, 12, 14, 15, 20, 23};

void	*routine(void *arg)
{
	int	index;
	int	sum;

	index = *(int *)arg;
	sum = 0;
	for (int j = 0; j < 5; j++)
		sum += primes[index + j];
	printf("local sum: %d\n", sum);
	*(int *)arg = sum;
	return (arg);
}

int	main(void)
{
	pthread_t	th[2];
	int			i;
	int			*a;
	int			globalsum;

	for (i = 0; i < 2; i++)
	{
		a = malloc(sizeof(int));
		*a = i * 5;
		if (pthread_create(&th[i], NULL, &routine, a) != 0)
			perror("Failed to created thread");
	}
	globalsum = 0;
	for (i = 0; i < 2; i++)
	{
		int* r;
		if (pthread_join(th[i], (void**)&r) != 0)
			perror("Failed to join thread");
		globalsum += *r;
		free(r);
	}
	printf("Global sum: %d\n", globalsum);
	return (0);
}
==============================================

pthread_mutex_t	mutex;
int				cont = 0;

void	*routine(void)
{
	int	i;

	i = -1;
	while (++i < 1000000)
	{
		pthread_mutex_lock(&mutex);
		cont++;
		pthread_mutex_unlock(&mutex);
	}
	return (NULL);
}

int	main(void)
{
	pthread_t tid1, tid2;
	pthread_mutex_init(&mutex, NULL);
	pthread_create(&tid1, NULL, (void *) &routine, NULL);
	pthread_create(&tid2, NULL, (void *) &routine, NULL);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	printf("cont: %d\n", cont);
	pthread_mutex_destroy(&mutex);
}
*/

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

int	is_digit_str(char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	check_input(int ac, char **av)
{
	int	i;
	int	philo_count;

	if (ac != 5 && ac != 6)
		return (0);
	i = 1;
	while (i < ac)
	{
		if (!is_digit_str(av[i]))
			return (0);
		i++;
	}
	philo_count = ft_atoi(av[1]);
	if (philo_count < 1 || philo_count > 200)
		return (0);
	if (ft_atoi(av[2]) <= 0 || ft_atoi(av[3]) <= 0 || ft_atoi(av[4]) <= 0)
		return (0);
	if (ac == 6 && ft_atoi(av[5]) < 0)
		return (0);
	return (1);
}

int	main(int ac, char **av)
{
	if (!check_input(ac, av))
	{
		printf("Invalid input. Usage: ./philo N t_die t_eat t_sleep [meals]\n");
		return (1);
	}
	printf("Input is valid ✅\n");
	return (0);
}
