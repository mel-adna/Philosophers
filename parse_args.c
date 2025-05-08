#include "philo.h"

int	ft_atoi(const char *str)
{
	int	res;
	int	sign;

	res = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		str++;
	}
	return (res * sign);
}

int	parse_input(int argc, char **argv, t_data *data);
{
    int i;
    if (argc != 5 && argc != 6)
        return (printf("Wrong number of arguments\n"));
    i = 1;
    while (i < argc)
    {
        if (is_numeric(argv[i]))
            return (printf("Arguments must be numeric\n"));
        i++;
    }
    return (0);
}
