#include "philo.h"

void	exit_(char *str)
{
	printf("Error : %s\n", str);
	exit (0);
}

void	print_message(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->d->i_take_printer);
	if (philo->d->everyone_is_healthy == 1)
		printf("%lld %d %s\n", get_time() - philo->d->t_start, philo->id + 1, str);
	pthread_mutex_unlock(&philo->d->i_take_printer);
}