/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <vburton@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 10:41:37 by vburton           #+#    #+#             */
/*   Updated: 2023/06/07 12:22:34 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	join_philo(pthread_t *threads, t_data *data)
{
	int	i;

	i = 0;
	while (data->nb_philo > 1 && i < data->nb_philo)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

void	clean_process(t_fork *fork, t_philo *philo, pthread_t *thread)
{
	free(fork);
	free(philo);
	free(thread);
}

void	clear_mutex(t_data *data, t_fork *forks, int nb_threads)
{
	int	i;

	i = 0;
	while (i < nb_threads)
	{
		pthread_mutex_lock(&forks[i].in_use);
		pthread_mutex_unlock(&forks[i].in_use);
		i++;
	}
	i = 0;
	pthread_mutex_lock(&data->glob);
	while (i < nb_threads)
	{
		pthread_mutex_destroy(&forks[i].in_use);
		i++;
	}
	pthread_mutex_unlock(&data->glob);
	pthread_mutex_destroy(&data->glob);
}
