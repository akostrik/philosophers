#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>

struct s_data;

// ph = philosopher
// phs = philosophers
typedef	struct			s_ph
{
	int					id;
	int					x_ate;
	int					left_fork_id;
	int					right_fork_id;
	long long			t_last_meal;
	struct s_data		*d;
	pthread_t			thread_id;
}						t_ph;

typedef struct			s_data
{
	int					nb_phs;
	int					t_death;
	int					t_eat;
	int					t_sleep;
	int					nb_eat;
	int					dead;
	int					everybody_has_eaten;
	long long			first_timestamp;
	pthread_mutex_t		meal_check;
	pthread_mutex_t		forks[200];
	pthread_mutex_t		writing;
	t_ph				phs[200];
}						t_data;

int						ft_atoi(const char *str);
void					action_print(t_data *d, int id, char *string);
long long				timestamp(void);
long long				time_diff(long long past, long long pres);
void					sleep_(long long time, t_data *d);
void					launcher(t_data *d);
void					exit_launcher(t_data *d, t_ph *phs);
void					exit_(char *str);

#endif
