#include "philo.h"

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

void	find_forks(t_data *d, int id)
{
	d->philos[id].l_fork = &d->forks[id];
	if (id + 1 >= d->nbr_philo)
		d->philos[id].r_fork = &d->forks[0];
	else
		d->philos[id].r_fork = &d->forks[id + 1];
}

void	start_half(t_data *d, int i)
{
	while (i < d->nbr_philo)
	{
		d->philos[i].d = d;
		d->philos[i].id = i;
		d->philos[i].last_eat = d->time;
		d->philos[i].limit_eat = d->time + d->t_die;
		d->philos[i].nbr_eat = 0;
		find_forks(d, i);
		pthread_mutex_init(&d->philos[i].m_eating, NULL);
		pthread_create(&d->philos[i].thread, NULL, philosopher,
			&d->philos[i]);
		i += 2;
	}
}

void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	print_message(philo, "has taken a fork");
	pthread_mutex_lock(philo->r_fork);
	print_message(philo, "has taken a fork");
	pthread_mutex_lock(&philo->m_eating);
	print_message(philo, "is eating");
	philo->last_eat = get_time();
	philo->limit_eat = philo->last_eat + philo->d->t_die;
	pthread_mutex_unlock(&philo->m_eating);
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
