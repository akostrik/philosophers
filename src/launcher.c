#include "philo.h"

static void	philo_eats(t_ph *ph)
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

static void	*philo_thread(void *void_ph)
{
	int		i;
	t_ph	*ph;

	ph = (t_ph *)void_ph;
	if (ph->id % 2)
		usleep(15000);
	i = 0;
	while (!((ph->d)->dead))
	{
		philo_eats(ph);
		if ((ph->d)->all_ate)
			break ;
		action_print(ph->d, ph->id, "is sleeping");
		sleep_((ph->d)->t_sleep, ph->d);
		action_print(ph->d, ph->id, "is thinking");
		i++;
	}
	return (NULL);
}

static void	life_checker(t_data *d, t_ph *ph)
{
	int i;

	while (!(d->all_ate))
	{
		i = -1;
		while (++i < d->nb_philo && !(d->dead))
		{
			pthread_mutex_lock(&(d->meal_check));
			if (time_diff(ph[i].t_last_meal, timestamp()) > d->t_death)
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
		while (d->nb_eat != -1 && ++i < d->nb_philo && ph[i].x_ate >= d->nb_eat)
			;
		if (i == d->nb_philo)
			d->all_ate = 1;
	}
}

void	launcher(t_data *d)
{
	int				i;

	d->first_timestamp = timestamp();
	i = -1;
	while (++i < d->nb_philo)
	{
		if (pthread_create(&((d->phs)[i].thread_id), NULL, philo_thread, &((d->phs)[i])))
			exit_("Threads creating");
		(d->phs)[i].t_last_meal = timestamp();
	}
	life_checker(d, d->phs);
	i = -1;
	while (++i < d->nb_philo)
		pthread_join(d->phs[i].thread_id, NULL);
	i = -1;
	while (++i < d->nb_philo)
		pthread_mutex_destroy(&(d->forks[i]));
	pthread_mutex_destroy(&(d->writing));
}
