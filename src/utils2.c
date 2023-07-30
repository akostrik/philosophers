#include "philo.h"

long long	get_time(void)
{
	static struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int nbr_meals_max_is_reached(t_data *d)
{
	int	i;
	int	to_return;

	if (d->nbr_meals_max == -1)
		return (0);
	i = -1;
	to_return = 1;
	pthread_mutex_lock(&d->i_take_count_journal);
	while (++i < d->nbr_phs)
		if (d->philos[i].nbr_meals < d->nbr_meals_max)
		{
			to_return = 0;
			break ;
		}
	pthread_mutex_unlock(&d->i_take_count_journal);
	return (to_return);
}

int get_health(t_data *d)
{
	int	to_return;
	
	to_return = 1;
	pthread_mutex_lock(&d->i_take_health_journal);
	if (d->health == 0)
		to_return = 0;
	pthread_mutex_unlock(&d->i_take_health_journal);
	return (to_return);
}

void set_health(t_data *d, int val)
{
	pthread_mutex_lock(&d->i_take_health_journal);
		d->health = val;
	pthread_mutex_unlock(&d->i_take_health_journal);
}

void	print_message(t_philo *ph, char *s)
{
	pthread_mutex_lock(&ph->d->i_take_printer);
	if ((get_health(ph->d) == 1 && nbr_meals_max_is_reached(ph->d) == 0) || s[0] == 'd')
		printf("%5lld %d %s\n", get_time() - ph->d->t_start, ph->id + 1, s);
	pthread_mutex_unlock(&ph->d->i_take_printer);
}
/* ************************************************************************** */