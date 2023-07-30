#include "philo.h"

void	*thread_philo(void *ph0)
{
	t_philo	*ph;

	ph = (t_philo *)ph0;
	while (get_health(ph->d) == 1 && nbr_meals_max_is_reached(ph->d) == 0)
	{
		pthread_mutex_lock(&(ph->d->i_take_fork[ph->id]));
		print_message(ph, "has taken a fork");
		pthread_mutex_lock(&(ph->d->i_take_fork[(ph->id + 1) % ph->d->nbr_phs]));
		print_message(ph, "has taken a fork");
		print_message(ph, "is eating");
		ph->t_next_meal = get_time() + ph->d->t_die;
		usleep(1000 * ph->d->t_eat);
		pthread_mutex_unlock(&(ph->d->i_take_fork[(ph->id + 1) % ph->d->nbr_phs]));
		pthread_mutex_unlock(&(ph->d->i_take_fork[ph->id]));
		pthread_mutex_lock(&(ph->d->i_take_count_journal));
		(ph->nbr_meals)++;
		pthread_mutex_unlock(&(ph->d->i_take_count_journal));
		print_message(ph, "is sleeping");
		usleep(1000 * ph->d->t_slp);
		print_message(ph, "is thinking");
	}
	return (NULL);
}

void	init(int argc, char const *argv[], t_data *d)
{
	int	i;

	if (argc <= 4 || argc >= 7)
		exit_("Error args");
	d->nbr_phs = ft_atoi(argv[1]);
	d->t_die = ft_atoi(argv[2]);
	d->t_eat = ft_atoi(argv[3]);
	d->t_slp = ft_atoi(argv[4]);
	d->nbr_meals_max = -1;
	if (argc == 6)
		d->nbr_meals_max = ft_atoi(argv[5]);
	if (d->nbr_phs <= 0 || d->t_eat <= 0 || d->t_slp <= 0 || d->t_die <= 0 || (argc == 6 && d->nbr_meals_max == -1))
		exit_("Error args 2");
	pthread_mutex_init(&d->i_take_printer, NULL);
	pthread_mutex_init(&d->i_take_health_journal, NULL);
	pthread_mutex_init(&d->i_take_count_journal, NULL);
	i = -1;
	while (++i < d->nbr_phs)
		pthread_mutex_init(&d->i_take_fork[i], NULL);
	d->health = 1;
}

void	start_threads(t_data *d)
{
	int	i;

	d->t_start = get_time();
	i = -1;
	while (++i < d->nbr_phs)
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
	while (++i < d->nbr_phs)
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
			set_health(&d, 0);
			print_message(&d.philos[i], "died");
			break ;
		}
		if (nbr_meals_max_is_reached(&d) == 1)
			break ;
		i = (i + 1) % d.nbr_phs;
	}
	i = -1;
	while (++i < d.nbr_phs)
		pthread_join(d.philos[i].thread, NULL);
	free_(&d);
	return (0);
}
