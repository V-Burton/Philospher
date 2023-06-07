/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <vburton@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 18:13:05 by vburton           #+#    #+#             */
/*   Updated: 2023/06/07 14:00:35 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

long	get_actual_time(long start_time)
{
	struct timeval	tv;
	long			time;

	gettimeofday(&tv, NULL);
	time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (time - start_time);
}

void	safe_printf(t_philo *philo, char *message)
{
	long	time;

	time = get_actual_time(philo->data->start_time);
	pthread_mutex_lock(&philo->data->glob);
	if (philo->data->run)
	{
		printf("%ld %d %s\n", time, philo->id, message);
	}
	pthread_mutex_unlock(&philo->data->glob);
}

int	is_odd(int i)
{
	if (i % 2 == 0)
		return (PAIR);
	return (ODD);
}

time_t	define_delta(long t2d, long t2s, long t2e)
{
	time_t	delta;

	delta = t2d - t2s - t2e;
	if (delta < 0)
		delta *= -1;
	return (delta);
}
