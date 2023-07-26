#include "philo.h"

static void	init_phs(t_data *d)
{
	int i;

	i = -1;
	while (++i < d->nb_philo)
	{
		d->phs[i].id = i;
		d->phs[i].x_ate = 0;
		d->phs[i].left_fork_id = i;
		d->phs[i].right_fork_id = (i + 1) % d->nb_philo;
		d->phs[i].t_last_meal = 0;
		d->phs[i].d = d; /// ?
		if (pthread_mutex_init(&(d->forks[i]), NULL))
			exit_("Intializing mutex");
	}
	if (pthread_mutex_init(&(d->writing), NULL))
		exit_("Intializing mutex");
	if (pthread_mutex_init(&(d->meal_check), NULL))
		exit_("Intializing mutex");
}

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
	d.dead = 0;
	init_phs(&d);
	launcher(&d);
	return (0);
}
