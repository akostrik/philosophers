#include "philo.h"

static void	ph_eats(t_ph *ph)
{
	pthread_mutex_lock(&((ph->d)->forks[ph->left_fork_id]));
	action_print((ph->d), ph->id, "has taken a fork");
	pthread_mutex_lock(&((ph->d)->forks[ph->right_fork_id]));
	action_print((ph->d), ph->id, "has taken a fork");
	pthread_mutex_lock(&((ph->d)->meal_check));
	action_print((ph->d), ph->id, "is eating");
	ph->t_last_meal = timestamp();
	pthread_mutex_unlock(&((ph->d)->meal_check));
	sleep_((ph->d)->t_eat, (ph->d));
	(ph->x_ate)++;
	pthread_mutex_unlock(&((ph->d)->forks[ph->left_fork_id]));
	pthread_mutex_unlock(&((ph->d)->forks[ph->right_fork_id]));
}

static void	*ph_thread(void *ph)
{
	if (((t_ph *)ph)->id % 2)
		usleep(15000);
	while (!((((t_ph *)ph)->d)->dead))
	{
		ph_eats(ph);
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

	while (!(d->everybody_has_eaten))
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
	int				i;

	d->first_timestamp = timestamp();
	i = -1;
	while (++i < d->nb_phs)
	{
		if (pthread_create(&((d->phs)[i].thread_id), NULL, ph_thread, &((d->phs)[i])))
			exit_("Threads creating");
		(d->phs)[i].t_last_meal = timestamp();
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
