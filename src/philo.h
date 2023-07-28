#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

struct s_data;

// ph = philosopher
// phs = philosophers
typedef	struct			s_ph
{
	int					id;
	int					nb_meals;
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
	int					nb_meals_max;
	int					evrybody_is_alive;
	int					everybody_has_got_nb_meals_max;
	long long			first_timestamp;
	pthread_mutex_t		check_if_everyone_is_alive;
	pthread_mutex_t		forks[200];
	pthread_mutex_t		writing;
	t_ph				phs[200];
}						t_data;

int						ft_atoi(const char *str);
void					print_action(t_data *d, int id, char *string);
long long				timestamp(void);
void					sleep_(long long time, t_data *d);
void					exit_(char *str);

#endif
