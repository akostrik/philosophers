/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimon <nsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 01:28:44 by nsimon            #+#    #+#             */
/*   Updated: 2021/08/03 01:32:06 by nsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_good(t_data *status)
{
	pthread_mutex_lock(&status->m_good);
	if (!status->good)
	{
		pthread_mutex_unlock(&status->m_good);
		return (1);
	}
	pthread_mutex_unlock(&status->m_good);
	return (0);
}
