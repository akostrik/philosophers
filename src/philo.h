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
	long long		t_last_meal;
	long long		t_next_meal;
	struct s_data	*d;
	pthread_mutex_t	*unused;
	pthread_mutex_t	*unused2;
	int				nbr_meals;
}	t_philo;

typedef struct s_data
{
	int				nbr_philo;
	int				t_die;
	int				t_eat;
	int				t_slp;
	long long		t_start;
	t_philo			philos[200];
	int				we_should_continue;
	pthread_mutex_t	i_take_fork[200];
	pthread_mutex_t	m_we_should_continue;
	pthread_mutex_t	i_take_printer;

	// int				eat_count;
	// pthread_mutex_t	m_eat_count;
	int				nbr_meals_max; //
}	t_data;

int			ft_strlen(const char *str);
int			ft_atoi(const char *str);
int			check_good(t_data *d);
long long	get_time(void);
void		start_philos(t_data *d);
void		ft_usleep(t_data *d, int stop_ms);
void		ft_putnbr_fd(int n, int fd);
void		print_message(t_philo *philo, char *str);
void		philo_eat(t_philo *philo);
void		exit_(char *str);
void		print_message(t_philo *philo, char *str);

#endif
