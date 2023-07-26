#include "philo.h"

static int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

static int	is_whitespace(char c)
{
	if (c == ' ')
		return (1);
	if (c == '\f')
		return (1);
	if (c == '\n')
		return (1);
	if (c == '\r')
		return (1);
	if (c == '\t')
		return (1);
	if (c == '\v')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	size_t	i;
	int		sign;
	int		to_return;

	i = 0;
	while (is_whitespace(str[i]) == 1)
		i++;
	sign = 1;
	if (str[i] == '-')
	{
		if (!ft_isdigit(str[i + 1]))
			return (0);
		sign = -1;
		i++;
	}
	if (str[i] == '+')
		i++;
	while (str[i] == '0')
		i++;
	to_return = 0;
	while (ft_isdigit(str[i]) == 1)
		to_return = to_return * 10 + str[i++] - '0';
	return (sign * to_return);
}

long long	timestamp(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

long long	time_diff(long long past, long long pres)
{
	return (pres - past);
}

void	sleep_(long long time, t_data *d)
{
	long long i;

	i = timestamp();
	while (!(d->dead))
	{
		if (time_diff(i, timestamp()) >= time)
			break ;
		usleep(50);
	}
}

void	action_print(t_data *d, int id, char *str)
{
	pthread_mutex_lock(&(d->writing));
	if (!(d->dead))
		printf("%lli %i %s\n", timestamp() - d->first_timestamp, id + 1, str);
	pthread_mutex_unlock(&(d->writing));
	return ;
}

void exit_(char *str)
{
	printf("Error : %s\n", str);
	exit (0);
}
