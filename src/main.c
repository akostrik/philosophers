#include "philo.h"

int	init_mutex(t_data *d)
{
	int i;

	i = d->nb_philo;
	while (--i >= 0)
	{
		if (pthread_mutex_init(&(d->forks[i]), NULL))
			return (1);
	}
	if (pthread_mutex_init(&(d->writing), NULL))
		return (1);
	if (pthread_mutex_init(&(d->meal_check), NULL))
		return (1);
	return (0);
}

static void	init_philos(t_data *d)
{
	int i;

	i = d->nb_philo;
	while (--i >= 0)
	{
		d->philosophers[i].id = i;
		d->philosophers[i].x_ate = 0;
		d->philosophers[i].left_fork_id = i;
		d->philosophers[i].right_fork_id = (i + 1) % d->nb_philo;
		d->philosophers[i].t_last_meal = 0;
		d->philosophers[i].d = d; /// ?
	}
}

// static int	error_manager(int error)
// {
// 	if (error == 2)
// 		exit_("Fatal error when intializing mutex");
// 	return (1);
// }

int		main(int argc, char **argv)
{
	t_data	d;

	if (argc < 5 || argc > 6) 
		exit_("arg (nb_philo > 0, nb_philo < 200, t_depth > 60, t_eat > 60, t_sleep > 60, [nb_eats > 0)])");
	d.nb_philo = ft_atoi(argv[1]);
	d.t_death = ft_atoi(argv[2]);
	d.t_eat = ft_atoi(argv[3]);
	d.t_sleep = ft_atoi(argv[4]);
	if (d.nb_philo < 1 || d.nb_philo > 200 || d.t_death < 60 || d.t_eat < 60 || d.t_sleep < 60 ) /// 2
		exit_("arg (nb_philo > 0, nb_philo < 200, t_depth > 60, t_eat > 60, t_sleep > 60, [nb_eats > 0)])");
	d.nb_eat = -1;
	if (argv[5])
	{
		d.nb_eat = ft_atoi(argv[5]);
		if (d.nb_eat <= 0)
			exit_("arg (nb_philo > 0, nb_philo < 200, t_depth > 60, t_eat > 60, t_sleep > 60, [nb_eats > 0)])");
	}
	d.all_ate = 0;
	d.dieded = 0;
	init_philos(&d);
	if (init_mutex(&d))
		return (2);
	if (launcher(&d))
		exit_("There was an error creating the threads");
	return (0);
}
