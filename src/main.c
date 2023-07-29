// 5  800 200 200 7

#include "philo.h"

int	monitor_count_meals(t_data *d)
{
	pthread_mutex_lock(&d->m_eat_count);
	if (d->eat_count >= d->nbr_philo * d->nbrEat && d->nbrEat != -1)
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
			if (monitor_life(d, i) || monitor_count_meals(d))
				return (NULL);
		}
		usleep(100);
	}
}

void	*philosopher(void *philo0)
{
	t_philo	*philo;

	philo = (t_philo *)philo0;
	while (1)
	{
		if (philo->d->nbrEat != -1
			&& philo->nbr_eat == philo->d->nbrEat)
			return (NULL);
		if (check_good(philo->d))
			return (NULL);
		philo_eat(philo);
		if (check_good(philo->d))
			return (NULL);
		print_message(philo, "is sleeping");
		ft_usleep(philo->d, philo->d->t_slp);
		if (check_good(philo->d))
			return (NULL);
		print_message(philo, "is thinking");
	}
}

void	start_philos(t_data *d)
{
	int	i;

	i = 0;
	while (i < d->nbr_philo)
	{
		d->philos[i].d = d;
		d->philos[i].id = i;
		d->philos[i].last_eat = d->time;
		d->philos[i].limit_eat = d->time + d->t_die;
		d->philos[i].nbr_eat = 0;
		d->philos[i].l_fork = &d->forks[i];
		d->philos[i].r_fork = &d->forks[(i + 1) % d->nbr_philo];
		pthread_create(&d->philos[i].thread, NULL, philosopher, &d->philos[i]);
		i++;
		usleep(10);
	}
}

void init1(int argc, char const *argv[], t_data *d)
{
	int	i;

	if (argc <= 4 || argc >= 6)
		exit_("Error inputs");
	d->nbr_philo = ft_atoi(argv[1]);
	d->t_die = ft_atoi(argv[2]);
	d->t_eat = ft_atoi(argv[3]);
	d->t_slp = ft_atoi(argv[4]);
	d->nbrEat = -1;
	if (argc == 6)
		d->nbrEat = ft_atoi(argv[5]);
	if (d->nbr_philo <= 0 || d->t_eat <= 0 || d->t_slp <= 0 || d->t_die <= 0 || (argc == 6 && d->nbrEat != -1))
		exit_("Error inputs");
	d->good = 1;
	d->eat_count = 0;
	pthread_mutex_init(&d->m_print, NULL);
	i = -1;
	while (++i < d->nbr_philo)
		pthread_mutex_init(&d->forks[i], NULL);
	pthread_mutex_init(&d->m_good, NULL);
	pthread_mutex_init(&d->m_eat_count, NULL);
	d->time = get_time();
}

int	main(int argc, char const *argv[])
{
	t_data	d;
	int		i;

	init1(argc, argv, &d);
	start_philos(&d);
	pthread_create(&d.monitor, NULL, monitor, &d); //
	pthread_join(d.monitor, NULL);
	i = -1;
	while (++i < d.nbr_philo)
		pthread_detach(d.philos[i].thread);
	while (++i < d.nbr_philo)
		pthread_mutex_destroy(&d.forks[i]); //
	return (0);
}
