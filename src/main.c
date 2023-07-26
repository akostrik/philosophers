#include "philo.h"

int	init_mutex(t_rules *rules)
{
	int i;

	i = rules->nb_philo;
	while (--i >= 0)
	{
		if (pthread_mutex_init(&(rules->forks[i]), NULL))
			return (1);
	}
	if (pthread_mutex_init(&(rules->writing), NULL))
		return (1);
	if (pthread_mutex_init(&(rules->meal_check), NULL))
		return (1);
	return (0);
}

int	init_philosophers(t_rules *rules)
{
	int i;

	i = rules->nb_philo;
	while (--i >= 0)
	{
		rules->philosophers[i].id = i;
		rules->philosophers[i].x_ate = 0;
		rules->philosophers[i].left_fork_id = i;
		rules->philosophers[i].right_fork_id = (i + 1) % rules->nb_philo;
		rules->philosophers[i].t_last_meal = 0;
		rules->philosophers[i].rules = rules;
	}
	return (0);
}

int	init_all(t_rules *rules, char **argv)
{
	rules->nb_philo = ft_atoi(argv[1]);
	rules->time_death = ft_atoi(argv[2]);
	rules->time_eat = ft_atoi(argv[3]);
	rules->time_sleep = ft_atoi(argv[4]);
	rules->all_ate = 0;
	rules->dieded = 0;
	if (rules->nb_philo < 1 || rules->time_death < 60 || rules->time_eat < 60
		|| rules->time_sleep < 60 || rules->nb_philo > 200) /// 2
		exit_("params (nb_philo > 0, nb_philo < 200, time_die > 60, time_eat > 60, time_sleep > 60)");
	if (argv[5])
	{
		rules->nb_eat = ft_atoi(argv[5]);
		if (rules->nb_eat <= 0)
			return (1);
	}
	else
		rules->nb_eat = -1;
	if (init_mutex(rules))
		return (2);
	init_philosophers(rules);
	return (0);
}

static int	error_manager(int error)
{
	if (error == 1)
		exit_("At least one wrong argument");
	if (error == 2)
		exit_("Fatal error when intializing mutex");
	return (1);
}

int		main(int argc, char **argv)
{
	t_rules	rules;
	int		ret;

	if (argc != 5 && argc != 6)
		exit_("Wrong amount of arguments");
	if ((ret = init_all(&rules, argv)))
		return (error_manager(ret));
	if (launcher(&rules))
		exit_("There was an error creating the threads");
	return (0);
}
