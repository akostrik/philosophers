#include "philo.h"

void	*thread_philo(void *philo0)
{
	t_philo	*philo;

	philo = (t_philo *)philo0;
	while (get_everyone_is_healthy(philo->d) == 1 && nbr_meals_max_is_reached(philo->d) == 0)
	{
		pthread_mutex_lock(&(philo->d->i_take_fork[philo->id]));
		print_message(philo, "has taken a fork");
		pthread_mutex_lock(&(philo->d->i_take_fork[(philo->id + 1) % philo->d->nbr_philo]));
		print_message(philo, "has taken a fork");
		print_message(philo, "is eating");
		philo->t_next_meal = get_time() + philo->d->t_die;
		usleep(1000 * philo->d->t_eat);
		pthread_mutex_unlock(&(philo->d->i_take_fork[(philo->id + 1) % philo->d->nbr_philo]));
		pthread_mutex_unlock(&(philo->d->i_take_fork[philo->id]));
		pthread_mutex_lock(&(philo->d->i_take_count_journal));
		(philo->nbr_meals)++;
		pthread_mutex_unlock(&(philo->d->i_take_count_journal));
		print_message(philo, "is sleeping");
		usleep(1000 * philo->d->t_slp);
		print_message(philo, "is thinking");
	}
	return (NULL);
}

void	init(int argc, char const *argv[], t_data *d)
{
	int	i;

	if (argc <= 4 || argc >= 7)
		exit_("Error args");
	d->nbr_philo = ft_atoi(argv[1]);
	d->t_die = ft_atoi(argv[2]);
	d->t_eat = ft_atoi(argv[3]);
	d->t_slp = ft_atoi(argv[4]);
	d->nbr_meals_max = -1;
	if (argc == 6)
		d->nbr_meals_max = ft_atoi(argv[5]);
	if (d->nbr_philo <= 0 || d->t_eat <= 0 || d->t_slp <= 0 || d->t_die <= 0 || (argc == 6 && d->nbr_meals_max == -1))
		exit_("Error args 2");
	pthread_mutex_init(&d->i_take_printer, NULL);
	pthread_mutex_init(&d->i_take_health_journal, NULL);
	pthread_mutex_init(&d->i_take_count_journal, NULL);
	i = -1;
	while (++i < d->nbr_philo)
		pthread_mutex_init(&d->i_take_fork[i], NULL);
	d->everyone_is_healthy = 1;
}

void	start_threads(t_data *d)
{
	int	i;

	d->t_start = get_time();
	i = -1;
	while (++i < d->nbr_philo)
	{
		d->philos[i].d = d;
		d->philos[i].id = i;
		d->philos[i].t_next_meal = d->t_start + d->t_die;
		d->philos[i].nbr_meals = 0;
		pthread_create(&d->philos[i].thread, NULL, thread_philo, &d->philos[i]);
		usleep(20);
	}
}

void	free_(t_data *d)
{
	int	i;

	i = -1;
	while (++i < d->nbr_philo)
		pthread_mutex_destroy(&d->i_take_fork[i]);
	pthread_mutex_destroy(&d->i_take_printer);
	pthread_mutex_destroy(&d->i_take_health_journal);
	pthread_mutex_destroy(&d->i_take_count_journal);
}

int	main(int argc, char const *argv[])
{
	t_data	d;
	int		i;

	init(argc, argv, &d);
	start_threads(&d);
	i = 0;
	while (1)
	{
		if (get_time() > d.philos[i].t_next_meal)
		{
			set_everyone_is_healthy(&d, 0);
			print_message(&d.philos[i], "died");
			break ;
		}
		if (nbr_meals_max_is_reached(&d) == 1)
			break ;
		i = (i + 1) % d.nbr_philo;
	}
	i = -1;
	while (++i < d.nbr_philo)
		pthread_join(d.philos[i].thread, NULL);
	free_(&d);
	return (0);
}
