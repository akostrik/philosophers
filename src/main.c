// 5  800 200 200 7

#include "philo.h"

int	everyone_is_healthy(t_data *d)
{
	if (d->everyone_is_healthy == 0)
		return (0);
	return (1);
}

void	sleep_as_long_as_everyone_is_healthy(t_data *d, int length)
{
	long long	t_end;

	t_end = get_time() + length;
	while (get_time() < t_end)
	{
		if (everyone_is_healthy(d) == 0)
			return ;
		usleep(100);
	}
}

void	*thread_philo(void *philo0)
{
	t_philo	*philo;

	philo = (t_philo *)philo0;
	while (1)
	{
		// if (philo->d->nbr_meals_max != -1 && philo->nbr_meals == philo->d->nbr_meals_max)
		// 	return (NULL);
		if (everyone_is_healthy(philo->d) == 0)
			return (NULL);
		pthread_mutex_lock(&(philo->d->i_take_fork[philo->id]));
		print_message(philo, "has taken a fork");
		pthread_mutex_lock(&(philo->d->i_take_fork[(philo->id + 1) % philo->d->nbr_philo]));
		print_message(philo, "has taken a fork");
		print_message(philo, "is eating");
		philo->t_last_meal = get_time();
		philo->t_next_meal = philo->t_last_meal + philo->d->t_die;
		sleep_as_long_as_everyone_is_healthy(philo->d, philo->d->t_eat);
		pthread_mutex_unlock(&(philo->d->i_take_fork[(philo->id + 1) % philo->d->nbr_philo]));
		pthread_mutex_unlock(&(philo->d->i_take_fork[philo->id]));
		// if (philo->d->nbr_meals_max != -1)
		// {
		// 	pthread_mutex_lock(&philo->d->m_eat_count);
		// 	philo->nbr_meals += 1;
		// 	philo->d->eat_count += 1;
		// 	pthread_mutex_unlock(&philo->d->m_eat_count);
		// }
		if (everyone_is_healthy(philo->d) == 0)
			return (NULL);
		print_message(philo, "is sleeping");
		sleep_as_long_as_everyone_is_healthy(philo->d, philo->d->t_slp);
		if (everyone_is_healthy(philo->d) == 0)
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
	// d->nbr_meals_max = -1;
	// if (argc == 6)
	// 	d->nbr_meals_max = ft_atoi(argv[5]);
	// if (d->nbr_philo <= 0 || d->t_eat <= 0 || d->t_slp <= 0 || d->t_die <= 0 || (argc == 6 && d->nbr_meals_max != -1))
	// 	exit_("Error inputs");
	d->everyone_is_healthy = 1;
	// d->eat_count = 0;
	pthread_mutex_init(&d->i_take_printer, NULL);
	// pthread_mutex_init(&d->i_take_dairy_of_health, NULL);
	// pthread_mutex_init(&d->m_eat_count, NULL);
	i = -1;
	while (++i < d->nbr_philo)
		pthread_mutex_init(&d->i_take_fork[i], NULL);
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
		d.philos[i].t_last_meal = d.t_start;
		d.philos[i].t_next_meal = d.t_start + d.t_die;
		// d.philos[i].nbr_meals = 0;
		d.philos[i].unused2 = &d.i_take_fork[(i + 1) % d.nbr_philo];
		pthread_create(&d.philos[i].thread, NULL, thread_philo, &d.philos[i]);
		usleep(20);
	}
	i = -1;
	while (1)
	{
		// pthread_mutex_lock(&d.m_eat_count);
		// if (d.eat_count >= d.nbr_philo * d.nbr_meals_max && d.nbr_meals_max != -1)
		// {
		// 	d.everyone_is_healthy = 0;
		// 	pthread_mutex_unlock(&d.m_eat_count);
		// 	break ;
		// }
		// pthread_mutex_unlock(&d.m_eat_count);
		if (get_time() > d.philos[i].t_next_meal)
		{
			d.everyone_is_healthy = 0;
			pthread_mutex_lock(&d.i_take_printer);
			printf("%lld %d died\n", get_time() - d.t_start, i + 1);
			pthread_mutex_unlock(&d.i_take_printer);
			return (1);
		}
		i = (i + 1) % d.nbr_philo;
		usleep(10);
	}
	i = -1;
	while (++i < d.nbr_philo)
		pthread_detach(d.philos[i].thread);
	while (++i < d.nbr_philo)
		pthread_mutex_destroy(&d.i_take_fork[i]);
	return (0);
}
