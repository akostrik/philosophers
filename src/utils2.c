#include "philo.h"

void	exit_(char *str)
{
	printf("Error : %s\n", str);
	exit (0);
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
	if (get_everyone_is_healthy(philo->d) == 1)
		printf("%lld %d %s\n", get_time() - philo->d->t_start, philo->id + 1, str);
	pthread_mutex_unlock(&philo->d->i_take_printer);
}