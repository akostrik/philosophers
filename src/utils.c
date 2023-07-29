/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimon <nsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 10:21:04 by nsimon            #+#    #+#             */
/*   Updated: 2021/08/03 00:58:15 by nsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	ft_usleep(t_main *status, int stop_ms)
{
	long long	end_ms;

	end_ms = get_time() + stop_ms;
	while (get_time() < end_ms)
	{
		pthread_mutex_lock(&status->m_good);
		if (status->good != 1)
		{
			pthread_mutex_unlock(&status->m_good);
			return ;
		}
		pthread_mutex_unlock(&status->m_good);
		usleep(100);
	}
}
