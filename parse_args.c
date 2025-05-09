#include "philo.h"

int	ft_atoi(const char *str)
{
	long long	res = 0;
	int			sign = 1;

	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		if ((sign == 1 && res > INT_MAX) || (sign == -1 && -res < INT_MIN))
			return (-1);
		str++;
	}
	return ((int)(res * sign));
}

static int	is_numeric(char *str)
{
	int i = 0;

	if (!str || str[0] == '\0')
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	print_error(char *msg)
{
	printf("Error: %s\n", msg);
	return (1);
}

int	parse_input(int argc, char **argv, t_data *data)
{
	int	i, value;

	i = 1;
	while (i < argc)
	{
		if (!is_numeric(argv[i]))
			return (print_error("Arguments must be numeric"));
		value = ft_atoi(argv[i]);
		if (value == -1)
			return (print_error("Argument too large or invalid"));
		i++;
	}
	data->nbr_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->meals_required = ft_atoi(argv[5]);
	else
		data->meals_required = -1;
	if (data->nbr_philos < 1 || data->nbr_philos > 200
		|| data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0 || (argc == 6 && data->meals_required <= 0))
		return (print_error("Invalid argument values"));
	return (data->someone_died = 0, 0);
}


