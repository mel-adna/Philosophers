#include "philo.h"

void	*monitor(void *arg)
{
	t_data	*data;
	int		i;
	int		all_finished;

	data = (t_data *)arg;
	while (!data->dead)
	{
		i = 0;
		all_finished = 1;
		while (i < data->philo_num)
		{
			if (data->philos[i].status != FINISHED)
				all_finished = 0;
			i++;
		}
		if (all_finished)
		{
			data->finished = 1;
			break ;
		}
		my_usleep(data,50);
	}
	return (NULL);
}
