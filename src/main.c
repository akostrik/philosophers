// 5  800 200 200 7

#include "philo.h"

void	*thread_philo(void *philo0)
{
	t_philo	*philo;

	philo = (t_philo *)philo0;
	while (1)
	{
		if (philo->d->nbrEat != -1 && philo->nbr_eat == philo->d->nbrEat)
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
	pthread_mutex_init(&d->i_take_printer, NULL);
	i = -1;
	while (++i < d->nbr_philo)
		pthread_mutex_init(&d->i_take_fork[i], NULL);
	pthread_mutex_init(&d->m_good, NULL);
	pthread_mutex_init(&d->m_eat_count, NULL);
	d->t_start = get_time();
}

int	main(int argc, char const *argv[])
{
	t_data	d;
	int		i;

	init1(argc, argv, &d);
	i = -1;
	while (++i < d.nbr_philo)
	{
		d.philos[i].d = &d;
		d.philos[i].id = i;
		d.philos[i].last_eat = d.t_start;
		d.philos[i].limit_eat = d.t_start + d.t_die;
		d.philos[i].nbr_eat = 0;
		d.philos[i].l_fork = &d.i_take_fork[i];
		d.philos[i].r_fork = &d.i_take_fork[(i + 1) % d.nbr_philo];
		pthread_create(&d.philos[i].thread, NULL, thread_philo, &d.philos[i]);
		usleep(10);
	}
	i = -1;
	while (1)
	{
		pthread_mutex_lock(&d.m_eat_count);
		if (d.eat_count >= d.nbr_philo * d.nbrEat && d.nbrEat != -1)
		{
			pthread_mutex_lock(&d.m_good);
			d.good = 0;
			pthread_mutex_unlock(&d.m_good);
			pthread_mutex_unlock(&d.m_eat_count);
			break ;
		}
		pthread_mutex_unlock(&d.m_eat_count);
		if (get_time() > d.philos[i].limit_eat)
		{
			pthread_mutex_lock(&d.m_good);
			d.good = 0;
			pthread_mutex_unlock(&d.m_good);
			pthread_mutex_lock(&d.i_take_printer);
			printf("%lld %d died\n", get_time() - d.t_start, i + 1);
			pthread_mutex_unlock(&d.i_take_printer);
			return (1);
		}
		i = (i + 1) % d.nbr_philo;
		if (i == 0)
			usleep(100);
	}
	i = -1;
	while (++i < d.nbr_philo)
		pthread_detach(d.philos[i].thread);
	while (++i < d.nbr_philo)
		pthread_mutex_destroy(&d.i_take_fork[i]);
	return (0);
}
