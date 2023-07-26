#include "philo.h"

// при одним философе не выходит

// pthread_mutex_init attr = NULL - the default mutex attributes are used
static void	init_phs(t_data *d)
{
	int	i;

	i = -1;
	while (++i < d->nb_phs)
	{
		d->phs[i].id = i;
		d->phs[i].x_ate = 0;
		d->phs[i].num_left_ph = i; // i -1
		d->phs[i].num_right_ph = (i + 1) % d->nb_phs;
		d->phs[i].t_last_meal = 0;
		d->phs[i].d = d; /// ?
		if (pthread_mutex_init(&(d->forks[i]), NULL))
			exit_("Intializing mutex");
	}
}

static void	*ph_thread(void *ph)
{
	if (((t_ph *)ph)->id % 2)
		usleep(15000);
	while (!((((t_ph *)ph)->d)->dead))
	{
		pthread_mutex_lock(&((((t_ph *)ph)->d)->forks[((t_ph *)ph)->num_left_ph]));
		action_print((((t_ph *)ph)->d), ((t_ph *)ph)->id, "has taken a fork");
		pthread_mutex_lock(&((((t_ph *)ph)->d)->forks[((t_ph *)ph)->num_right_ph]));
		action_print((((t_ph *)ph)->d), ((t_ph *)ph)->id, "has taken a fork");
		pthread_mutex_lock(&((((t_ph *)ph)->d)->meal_check));
		action_print((((t_ph *)ph)->d), ((t_ph *)ph)->id, "is eating");
		((t_ph *)ph)->t_last_meal = timestamp();
		pthread_mutex_unlock(&((((t_ph *)ph)->d)->meal_check));
		sleep_((((t_ph *)ph)->d)->t_eat, (((t_ph *)ph)->d));
		(((t_ph *)ph)->x_ate)++;
		pthread_mutex_unlock(&((((t_ph *)ph)->d)->forks[((t_ph *)ph)->num_left_ph]));
		pthread_mutex_unlock(&((((t_ph *)ph)->d)->forks[((t_ph *)ph)->num_right_ph]));
		if ((((t_ph *)ph)->d)->everybody_has_eaten)
			break ;
		action_print(((t_ph *)ph)->d, ((t_ph *)ph)->id, "is sleeping");
		sleep_((((t_ph *)ph)->d)->t_sleep, ((t_ph *)ph)->d);
		action_print(((t_ph *)ph)->d, ((t_ph *)ph)->id, "is thinking");
	}
	return (NULL);
}

static void	life_checker(t_data *d)
{
	int i;

	while (d->everybody_has_eaten == 1)
	{
		i = -1;
		while (++i < d->nb_phs && !(d->dead))
		{
			pthread_mutex_lock(&(d->meal_check));
			if (time_diff(d->phs[i].t_last_meal, timestamp()) > d->t_death)
			{
				action_print(d, i, "died");
				d->dead = 1;
			}
			pthread_mutex_unlock(&(d->meal_check));
			usleep(100);
		}
		if (d->dead)
			break ;
		i = -1;
		while (d->nb_eat != -1 && ++i < d->nb_phs && d->phs[i].x_ate >= d->nb_eat)
			;
		if (i == d->nb_phs)
			d->everybody_has_eaten = 1;
	}
}

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
	life_checker(d);
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
		exit_("arg (nb_philo > 0, nb_philo < 200, t_depth > 0, t_eat > 0, t_sleep > 0, [nb_eats > 0)])");
	d.nb_phs = ft_atoi(argv[1]);
	d.t_death = ft_atoi(argv[2]);
	d.t_eat = ft_atoi(argv[3]);
	d.t_sleep = ft_atoi(argv[4]);
	if (d.nb_phs < 1 || d.nb_phs > 200 || d.t_death < 0 || d.t_eat < 0 || d.t_sleep < 0 ) /// 2
		exit_("arg (nb_philo > 0, nb_philo < 200, t_depth > 0, t_eat > 0, t_sleep > 0, [nb_eats > 0)])");
	d.nb_eat = -1;
	if (argv[5] != NULL)
	{
		d.nb_eat = ft_atoi(argv[5]);
		if (d.nb_eat <= 0)
			exit_("arg (nb_philo > 0, nb_philo < 200, t_depth > 0, t_eat > 0, t_sleep > 0, [nb_eats > 0)])");
	}
	d.everybody_has_eaten = 0;
	d.dead = 0;
	if (pthread_mutex_init(&(d.writing), NULL))
		exit_("Intializing mutex");
	if (pthread_mutex_init(&(d.meal_check), NULL))
		exit_("Intializing mutex");
	init_phs(&d);
	launcher(&d);
	return (0);
}
