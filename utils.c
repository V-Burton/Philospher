/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 18:13:05 by vburton           #+#    #+#             */
/*   Updated: 2023/04/27 19:53:27 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

long get_actual_time()
{
	struct	timeval tv;
	long	time;

	gettimeofday(&tv, NULL);
	time = tv.tv_sec * 1000000 + tv.tv_usec;
	return (time);
}

void	safe_printf(t_philo *philo, char *message)
{
	pthread_mutex_lock(&philo->data->write);
	printf("Philosopher");
	printf(" %d ", philo->id);
	printf("%s.\n", message);
	pthread_mutex_unlock(&philo->data->write);
}

int is_odd(int i)
{
	if (i % 2 == 0)
		return (PAIR);
	return (ODD);
}