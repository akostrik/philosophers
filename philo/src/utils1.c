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

void	exit_(char *str)
{
	printf("Error : %s\n", str);
	exit (0);
}

void	print_message(t_philo *ph, char *s)
{
	pthread_mutex_lock(&ph->d->get_printer);
	if ((get_health(ph->d) == 1 && nb_meals_max_is_reached(ph->d) == 0) \
		|| s[0] == 'd')
		printf("%5lld %3d %s\n", get_time() - ph->d->t_start, ph->id + 1, s);
	pthread_mutex_unlock(&ph->d->get_printer);
}
