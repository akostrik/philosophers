/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02
/24 11:56:54 by akostrik          #+#    #+#             */
/*   Updated: 2023/07/23 16:14:01 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	static struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	nb_meals_max_is_reached(t_data *d)
{
	int	i;
	int	to_return;

	if (d->nb_meals_max == -1)
		return (0);
	to_return = 1;
	pthread_mutex_lock(&d->get_count_journal);
	i = -1;
	while (++i < d->nb_phs)
	{
		if ((d->phs[i].nb_meals) < (d->nb_meals_max))
		{
			to_return = 0;
			break ;
		}
	}
	pthread_mutex_unlock(&d->get_count_journal);
	return (to_return);
}

int	get_health(t_data *d)
{
	int	to_return;

	to_return = 1;
	pthread_mutex_lock(&d->get_health_journal);
	if (d->health == 0)
		to_return = 0;
	pthread_mutex_unlock(&d->get_health_journal);
	return (to_return);
}

void	set_health(t_data *d, int val)
{
	pthread_mutex_lock(&d->get_health_journal);
	d->health = val;
	pthread_mutex_unlock(&d->get_health_journal);
}
