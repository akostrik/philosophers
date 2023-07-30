// pthread_join приостанавливает выполнение вызывающего до завершения thread

// pthread_detach уведомляет реализацию, что область памяти для thread
// может быть восстановлена, когда он завершит выполнение
// Если thread не завершается, pthread_detach () не завершает его

// процессы имеют отдельные адресные пространства
// потоки совместно используют их адресное пространство ??

#include "philo.h"

void	*thread_philo(void *ph0)
{
	t_philo	*ph;

	ph = (t_philo *)ph0;
	while (get_health(ph->d) == 1 && nb_meals_max_is_reached(ph->d) == 0)
	{
		pthread_mutex_lock(&(ph->d->get_fork[ph->id]));
		print_message(ph, "has taken a fork");
		if (ph->d->nb_phs == 1)
			return (NULL);
		pthread_mutex_lock(&(ph->d->get_fork[(ph->id + 1) % ph->d->nb_phs]));
		print_message(ph, "has taken a fork");
		print_message(ph, "is eating");
		ph->t_next_meal = get_time() + ph->d->t_die;
		usleep(1000 * ph->d->t_eat);
		pthread_mutex_unlock(&(ph->d->get_fork[(ph->id + 1) % ph->d->nb_phs]));
		pthread_mutex_unlock(&(ph->d->get_fork[ph->id]));
		pthread_mutex_lock(&(ph->d->get_count_journal));
		(ph->nb_meals)++;
		pthread_mutex_unlock(&(ph->d->get_count_journal));
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
	d->nb_phs = ft_atoi(argv[1]);
	d->t_die = ft_atoi(argv[2]);
	d->t_eat = ft_atoi(argv[3]);
	d->t_slp = ft_atoi(argv[4]);
	if (d->nb_phs <= 0 || d->t_eat <= 0 || d->t_slp <= 0 || d->t_die <= 0)
		exit_("Error args");
	d->nb_meals_max = -1;
	if (argc == 6)
	{
		d->nb_meals_max = ft_atoi(argv[5]);
		if (d->nb_meals_max == -1)
			exit_("Error args 2");
	}
	pthread_mutex_init(&d->get_printer, NULL);
	pthread_mutex_init(&d->get_health_journal, NULL);
	pthread_mutex_init(&d->get_count_journal, NULL);
	i = -1;
	while (++i < d->nb_phs)
		pthread_mutex_init(&d->get_fork[i], NULL);
	d->health = 1;
}

void	start_threads(t_data *d)
{
	int	i;

	d->t_start = get_time();
	i = -1;
	while (++i < d->nb_phs)
	{
		d->phs[i].d = d;
		d->phs[i].id = i;
		d->phs[i].t_next_meal = d->t_start + d->t_die;
		d->phs[i].nb_meals = 0;
		pthread_create(&d->phs[i].thread, NULL, thread_philo, &d->phs[i]);
		usleep(20);
	}
}

void	free_(t_data *d)
{
	int	i;

	i = -1;
	while (++i < d->nb_phs)
		pthread_mutex_destroy(&d->get_fork[i]);
	pthread_mutex_destroy(&d->get_printer);
	pthread_mutex_destroy(&d->get_health_journal);
	pthread_mutex_destroy(&d->get_count_journal);
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
		if (get_time() > d.phs[i].t_next_meal)
		{
			set_health(&d, 0);
			print_message(&d.phs[i], "died");
			break ;
		}
		if (nb_meals_max_is_reached(&d) == 1)
			break ;
		i = (i + 1) % d.nb_phs;
	}
	i = -1;
	while (++i < d.nb_phs)
		pthread_join(d.phs[i].thread, NULL);
	free_(&d);
	return (0);
}
