#include "philo.h"

// при одним философе не выходит

// pthread_mutex_init attr = NULL - the default mutex attributes
static void	init_phs(t_data *d)
{
	int	i;

	i = -1;
	while (++i < d->nb_phs)
	{
		d->phs[i].id = i;
		d->phs[i].how_many_meals = 0;
		d->phs[i].t_last_meal = 0;
		d->phs[i].d = d; /// ?
		if (pthread_mutex_init(&(d->forks[i]), NULL))
			exit_("Intializing mutex");
	}
}

static void	*ph_thread(void *ph0)
{
	t_ph	*ph;

	ph = (t_ph *)ph0;
	if (ph->id % 2)
		usleep(15000); // связано с t_eat ...
	while (ph->d->evrybody_is_alive == 1)
	{
		pthread_mutex_lock(&((ph->d)->forks[ph->id]));
		print_action(ph->d, ph->id, "has taken a fork");
		pthread_mutex_lock(&((ph->d)->forks[(ph->id + 1) % ph->d->nb_phs]));
		print_action(ph->d, ph->id, "has taken a fork");
		pthread_mutex_lock(&((ph->d)->check_if_everyone_is_alive));
		print_action(ph->d, ph->id, "is eating");
		ph->t_last_meal = timestamp();
		pthread_mutex_unlock(&((ph->d)->check_if_everyone_is_alive));
		sleep_(ph->d->t_eat, ph->d);
		(ph->how_many_meals)++;
		printf("*** %d has eaten %d times\n",ph->id,ph->how_many_meals);
		pthread_mutex_unlock(&((ph->d)->forks[ph->id]));
		pthread_mutex_unlock(&((ph->d)->forks[(ph->id + 1) % ph->d->nb_phs]));
		if (ph->d->everybody_has_eaten)
			break ;
		print_action(ph->d, ph->id, "is sleeping");
		sleep_(ph->d->t_sleep, ph->d);
		print_action(ph->d, ph->id, "is thinking");
	}
	return (NULL);
}

static void	sleep_as_lons_as_everyone_is_alive(t_data *d)
{
	int i;

	while (d->everybody_has_eaten == 1)
	{
		i = -1;
		while (++i < d->nb_phs && d->evrybody_is_alive == 1)
		{
			pthread_mutex_lock(&(d->check_if_everyone_is_alive));
			if (timestamp() - d->phs[i].t_last_meal > d->t_death)
			{
				print_action(d, i, "died");
				d->evrybody_is_alive = 0;
			}
			pthread_mutex_unlock(&(d->check_if_everyone_is_alive));
			usleep(100);
		}
		if (!(d->evrybody_is_alive))
			break ;
		d->everybody_has_eaten = 1;
		i = -1;
		while (d->how_many_meals_max != -1 && ++i < d->nb_phs)
			if (d->phs[i].how_many_meals < d->how_many_meals_max)
			{
				d->everybody_has_eaten = 0;
				break ;
			}
	}
}

// stores ID of  the  new thread in the buffer pointed to by thread
// ID used to refer to the thread in subsequent calls to other pthreads functions

void	launcher(t_data *d)
{
	int	i;

	d->first_timestamp = timestamp();
	i = -1;
	while (++i < d->nb_phs)
	{
		if (pthread_create(&(d->phs[i].thread_id), NULL, ph_thread, &(d->phs[i])))
			exit_("Threads creating");
		d->phs[i].t_last_meal = timestamp();
	}
	sleep_as_lons_as_everyone_is_alive(d);
	i = -1;
	while (++i < d->nb_phs)
		pthread_join(d->phs[i].thread_id, NULL);
	i = -1;
	while (++i < d->nb_phs)
		pthread_mutex_destroy(&(d->forks[i]));
	pthread_mutex_destroy(&(d->writing));
}

int		main(int argc, char **argv)
{
	t_data	d;

	if (argc < 5 || argc > 6) 
		exit_("arg (0 < nb_philo < 200, die > 0, eat > 0, sleep > 0, [how_many_times_max > 0])");
	d.nb_phs = ft_atoi(argv[1]);
	d.t_death = ft_atoi(argv[2]);
	d.t_eat = ft_atoi(argv[3]);
	d.t_sleep = ft_atoi(argv[4]);
	if (d.nb_phs < 1 || d.nb_phs > 200 || d.t_death < 0 || d.t_eat < 0 || d.t_sleep < 0 ) /// 2
		exit_("arg (0 < nb_philo < 200, die > 0, eat > 0, sleep > 0, [how_many_times_max > 0])");
	d.how_many_meals_max = -1;
	if (argv[5] != NULL)
	{
		d.how_many_meals_max = ft_atoi(argv[5]);
		if (d.how_many_meals_max <= 0)
			exit_("arg (0 < nb_philo < 200, die > 0, eat > 0, sleep > 0, [how_many_times_max > 0])");
	}
	d.everybody_has_eaten = 0;
	d.evrybody_is_alive = 1;
	if (pthread_mutex_init(&(d.writing), NULL))
		exit_("Intializing mutex");
	if (pthread_mutex_init(&(d.check_if_everyone_is_alive), NULL))
		exit_("Intializing mutex");
	init_phs(&d);
	launcher(&d);
	return (0);
}
