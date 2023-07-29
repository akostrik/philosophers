#include "philo.h"

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

int	ft_atoi(const char *str)
{
	int	res;
	int	i;

	res = 0;
	i = 0;
	if (!str)
		return (res);
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res);
}

void	ft_putnbr_fd(int n, int fd)
{
	int		i;
	char	tmp[12];

	i = 0;
	while (n != 0)
	{
		tmp[i] = n % 10;
		if (tmp[i] < 0)
			tmp[i] = -tmp[i] + '0';
		else
			tmp[i] += '0';
		i++;
		if (n / 10 == 0 && n < 0)
			tmp[i++] = '-';
		n /= 10;
	}
	if (i == 0)
		tmp[i++] = '0';
	tmp[i] = '\0';
	while (i != 0)
		write(fd, &tmp[--i], 1);
}

long long	get_time(void)
{
	static struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	sleep_as_long_as_everyone_is_healthy(t_data *d, int stop_ms)
{
	long long	end_ms;

	end_ms = get_time() + stop_ms;
	while (get_time() < end_ms)
	{
		pthread_mutex_lock(&d->i_take_dairy_of_health);
		if (d->everyone_is_healthy == 0)
		{
			pthread_mutex_unlock(&d->i_take_dairy_of_health);
			return ;
		}
		pthread_mutex_unlock(&d->i_take_dairy_of_health);
		usleep(100);
	}
}

