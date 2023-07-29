#ifndef PHILO_H
# define PHILO_H

# include <stddef.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	long long		last_eat;
	long long		limit_eat;
	int				nbr_eat;
	pthread_mutex_t	m_eating;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	struct s_data	*d;
}	t_philo;

typedef struct s_data
{
	int				nbr_philo;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				nbrEat;
	int				good;
	int				eat_count;
	t_philo			philos[200];
	pthread_mutex_t	forks[200];
	long long		time;
	pthread_t		monitor;
	pthread_mutex_t	m_eat_count;
	pthread_mutex_t	m_good;
	pthread_mutex_t	m_dead;
	pthread_mutex_t	m_print;
}	t_data;

typedef struct s_args
{
	t_data	*d;
	t_philo	*philo;
}	t_args;

int			ft_strlen(const char *str);
int			ft_atoi(const char *str);
int			check_good(t_data *d);
long long	get_time(void);
void		*philosopher(void *arg);
void		start_half(t_data *d, int i);
void		ft_usleep(t_data *d, int stop_ms);
void		ft_putnbr_fd(int n, int fd);
void		print_message(t_philo *philo, char *str);
void		philo_eat(t_philo *philo);
void		*monitor(void *arg);
void		exit_(char *str);

#endif
