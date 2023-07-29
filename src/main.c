// 5  800 200 200 7

#include "philo.h"

void	*philosopher(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
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
		ft_usleep(philo->d, philo->d->timeToSleep);
		if (check_good(philo->d))
			return (NULL);
		print_message(philo, "is thinking");
	}
}

void	ft_clear(t_main *d)
{
	int	i;

	i = -1;
	while (++i < d->nbr_philo)
		pthread_detach(d->philos[i].thread);
	while (++i < d->nbr_philo)
		pthread_mutex_destroy(&d->forks[i]);
}

int	create_philo(t_main *d)
{
	int	i;

	d->eat_count = 0;
	pthread_mutex_init(&d->m_print, NULL);
	i = -1;
	while (++i < d->nbr_philo)
		pthread_mutex_init(&d->forks[i], NULL);
	pthread_mutex_init(&d->m_good, NULL);
	pthread_mutex_init(&d->m_eat_count, NULL);
	d->time = get_time();
	start_half(d, 0);
	ft_usleep(d, 10);
	start_half(d, 1);
	pthread_create(&d->monitor, NULL, monitor, d);
	pthread_join(d->monitor, NULL);
	ft_clear(d);
	return (0);
}

int	check_inputs(t_main *d)
{
	if (!(d->nbr_philo > 0))
		return (1);
	if (!(d->timeToEat > 0))
		return (1);
	if (!(d->timeToSleep > 0))
		return (1);
	if (!(d->timeToDie > 0))
		return (1);
	if (!(d->nbrEat > 0) && d->nbrEat != -1)
		return (1);
	return (0);
}

int	main(int argc, char const *argv[])
{
	t_main	d;

	if (argc <= 4 || argc >= 6)
	{
		write(STDERR_FILENO, "Error inputs\n", ft_strlen("Error inputs\n"));
		return (1);
	}
	d.nbr_philo = ft_atoi(argv[1]);
	d.timeToDie = ft_atoi(argv[2]);
	d.timeToEat = ft_atoi(argv[3]);
	d.timeToSleep = ft_atoi(argv[4]);
	d.nbrEat = -1;
	if (argc == 6)
		d.nbrEat = ft_atoi(argv[5]);
	if (check_inputs(&d))
	{
		write(STDERR_FILENO, "Error inputs\n", ft_strlen("Error inputs\n"));
		return (1);
	}
	d.good = 1;
	create_philo(&d);
	return (0);
}
