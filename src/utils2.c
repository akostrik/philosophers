#include "philo.h"

int	check_good(t_data *d)
{
	pthread_mutex_lock(&d->m_good);
	if (!d->good)
	{
		pthread_mutex_unlock(&d->m_good);
		return (1);
	}
	pthread_mutex_unlock(&d->m_good);
	return (0);
}

void	exit_(char *str)
{
	printf("Error : %s\n", str);
	exit (0);
}