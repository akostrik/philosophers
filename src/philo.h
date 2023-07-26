#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>

struct s_data;

typedef	struct			s_philosopher
{
	int					id;
	int					x_ate;
	int					left_fork_id;
	int					right_fork_id;
	long long			t_last_meal;
	struct s_data		*d;
	pthread_t			thread_id;
}						t_philosopher;

typedef struct			s_data
{
	int					nb_philo;
	int					t_death;
	int					t_eat;
	int					t_sleep;
	int					nb_eat;
	int					dieded;
	int					all_ate;
	long long			first_timestamp;
	pthread_mutex_t		meal_check;
	pthread_mutex_t		forks[200];
	pthread_mutex_t		writing;
	t_philosopher		philosophers[200];
}						t_data;

int						ft_atoi(const char *str);
void					action_print(t_data *d, int id, char *string);
long long				timestamp(void);
long long				time_diff(long long past, long long pres);
void					smart_sleep(long long time, t_data *d);
int						launcher(t_data *d);
void					exit_launcher(t_data *d, t_philosopher *philos);
void					exit_(char *str);

#endif
