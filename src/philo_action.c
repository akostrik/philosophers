#include "philo.h"

void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	print_message(philo, "has taken a fork");
	pthread_mutex_lock(philo->r_fork);
	print_message(philo, "has taken a fork");
	print_message(philo, "is eating");
	philo->last_eat = get_time();
	philo->limit_eat = philo->last_eat + philo->d->t_die;
	ft_usleep(philo->d, philo->d->t_eat);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
	if (philo->d->nbrEat != -1)
	{
		pthread_mutex_lock(&philo->d->m_eat_count);
		philo->nbr_eat += 1;
		philo->d->eat_count += 1;
		pthread_mutex_unlock(&philo->d->m_eat_count);
	}
}
