#include "philo.h"

int	monitor_eat(t_data *d)
{
	pthread_mutex_lock(&d->m_eat_count);
	if (d->eat_count >= d->nbr_philo * d->nbrEat
		&& d->nbrEat != -1)
	{
		pthread_mutex_lock(&d->m_good);
		d->good = 0;
		pthread_mutex_unlock(&d->m_good);
		pthread_mutex_unlock(&d->m_eat_count);
		return (1);
	}
	pthread_mutex_unlock(&d->m_eat_count);
	return (0);
}

int	monitor_life(t_data *d, int i)
{
	if (get_time() > d->philos[i].limit_eat)
	{
		pthread_mutex_lock(&d->m_print);
		pthread_mutex_lock(&d->m_good);
		d->good = 0;
		pthread_mutex_unlock(&d->m_good);
		printf("%lld %d died\n", get_time() - d->time, i + 1);
		pthread_mutex_unlock(&d->m_print);
		return (1);
	}
	return (0);
}

void	*monitor(void *arg)
{
	t_data		*d;
	int			i;

	d = (t_data *)arg;
	while (1)
	{
		i = -1;
		while (++i < d->nbr_philo)
		{
			pthread_mutex_lock(&d->philos[i].m_eating);
			if (monitor_life(d, i) || monitor_eat(d))
			{
				pthread_mutex_unlock(&d->philos[i].m_eating);
				return (NULL);
			}
			pthread_mutex_unlock(&d->philos[i].m_eating);
		}
		usleep(100);
	}
}
