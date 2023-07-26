#include "philo.h"

void	philo_eats(t_philosopher *philo)
{
	t_data *d; ///

	d = philo->d;
	pthread_mutex_lock(&(d->forks[philo->left_fork_id]));
	action_print(d, philo->id, "has taken a fork");
	pthread_mutex_lock(&(d->forks[philo->right_fork_id]));
	action_print(d, philo->id, "has taken a fork");
	pthread_mutex_lock(&(d->meal_check));
	action_print(d, philo->id, "is eating");
	philo->t_last_meal = timestamp();
	pthread_mutex_unlock(&(d->meal_check));
	smart_sleep(d->t_eat, d);
	(philo->x_ate)++;
	pthread_mutex_unlock(&(d->forks[philo->left_fork_id]));
	pthread_mutex_unlock(&(d->forks[philo->right_fork_id]));
}

void	*p_thread(void *void_philosopher)
{
	int				i;
	t_philosopher	*philo;
	t_data			*d;

	philo = (t_philosopher *)void_philosopher;
	d = philo->d;
	if (philo->id % 2)
		usleep(15000);
	i = 0;
	while (!(d->dieded))
	{
		philo_eats(philo);
		if (d->all_ate)
			break ;
		action_print(d, philo->id, "is sleeping");
		smart_sleep(d->t_sleep, d);
		action_print(d, philo->id, "is thinking");
		i++;
	}
	return (NULL);
}

static void	life_checker(t_data *d, t_philosopher *p)
{
	int i;

	while (!(d->all_ate))
	{
		i = -1;
		while (++i < d->nb_philo && !(d->dieded))
		{
			pthread_mutex_lock(&(d->meal_check));
			if (time_diff(p[i].t_last_meal, timestamp()) > d->t_death)
			{
				action_print(d, i, "died");
				d->dieded = 1;
			}
			pthread_mutex_unlock(&(d->meal_check));
			usleep(100);
		}
		if (d->dieded)
			break ;
		i = -1;
		while (d->nb_eat != -1 && ++i < d->nb_philo && p[i].x_ate >= d->nb_eat)
			;
		if (i == d->nb_philo)
			d->all_ate = 1;
	}
}

void	launcher(t_data *d)
{
	int				i;
	t_philosopher	*phi;

	phi = d->philosophers;
	d->first_timestamp = timestamp();
	i = -1;
	while (++i < d->nb_philo)
	{
		if (pthread_create(&(phi[i].thread_id), NULL, p_thread, &(phi[i])))
			exit_("Threads creating");
		phi[i].t_last_meal = timestamp();
	}
	life_checker(d, d->philosophers);
	i = -1;
	while (++i < d->nb_philo)
		pthread_join(d->philosophers[i].thread_id, NULL);
	i = -1;
	while (++i < d->nb_philo)
		pthread_mutex_destroy(&(d->forks[i]));
	pthread_mutex_destroy(&(d->writing));
}
