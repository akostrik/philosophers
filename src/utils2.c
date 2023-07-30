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
	while (++i < d->nbr_philo)
		if (d->philos[i].nbr_meals < d->nbr_meals_max)
		{
			to_return = 0;
			break ;
		}
	pthread_mutex_unlock(&d->i_take_count_journal);
	return (to_return);
}

int get_everyone_is_healthy(t_data *d)
{
	int	to_return;
	
	to_return = 1;
	pthread_mutex_lock(&d->i_take_health_journal);
	if (d->everyone_is_healthy == 0)
		to_return = 0;
	pthread_mutex_unlock(&d->i_take_health_journal);
	return (to_return);
}

void set_everyone_is_healthy(t_data *d, int val)
{
	pthread_mutex_lock(&d->i_take_health_journal);
		d->everyone_is_healthy = val;
	pthread_mutex_unlock(&d->i_take_health_journal);
}

void	print_message(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->d->i_take_printer);
	if ((get_everyone_is_healthy(philo->d) == 1 && nbr_meals_max_is_reached(philo->d) == 0) || str[0] == 'd')
		printf("%5lld %d %s\n", get_time() - philo->d->t_start, philo->id + 1, str);
	pthread_mutex_unlock(&philo->d->i_take_printer);
}