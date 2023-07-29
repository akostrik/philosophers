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

void	print_message(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->d->m_print);
	if (philo->d->good == 1)
	{
		printf("%lld %d %s\n", get_time() - philo->d->time,
			philo->id + 1, str);
	}
	pthread_mutex_unlock(&philo->d->m_print);
}