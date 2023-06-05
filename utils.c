/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <vburton@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 18:13:05 by vburton           #+#    #+#             */
/*   Updated: 2023/06/05 12:17:11 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

long get_actual_time()
{
	static long start_time;
	struct	timeval tv;
	long	time;

	gettimeofday(&tv, NULL);
	time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	if (start_time == 0)
		start_time = time;
	return (time - start_time);
}

void	safe_printf(t_philo *philo, char *message)
{
	if (philo->data->running)
	{
		pthread_mutex_lock(&philo->data->glob);
		printf("%ld %d %s\n", get_actual_time(), philo->id, message);
		pthread_mutex_unlock(&philo->data->glob);
	}
}

int	is_odd(int i)
{
	if (i % 2 == 0)
		return (PAIR);
	return (ODD);
}
