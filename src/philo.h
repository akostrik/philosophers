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
	int				everyone_is_healthy;
	pthread_mutex_t	i_take_fork[200];
	pthread_mutex_t	i_take_printer;
	pthread_mutex_t	i_take_health_journal;
	// int				eat_count;
	// pthread_mutex_t	m_eat_count;
	int				nbr_meals_max; //
}	t_data;

int			ft_strlen(const char *str);
int			ft_atoi(const char *str);
void		ft_putnbr_fd(int n, int fd);
long long	get_time(void);
void		sleep_but_wake_up_if_smb_is_dead(t_data *d, int stop_ms);
void		print_message(t_philo *philo, char *str);
void		exit_(char *str);
int			get_everyone_is_healthy(t_data *d);
void		set_everyone_is_healthy(t_data *d, int val);

#endif
