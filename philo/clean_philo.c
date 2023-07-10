/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <vburton@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 10:41:37 by vburton           #+#    #+#             */
/*   Updated: 2023/07/10 13:45:32 by vburton          ###   ########.fr       */
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
	if (fork)
		free(fork);
	if (thread)
		free(thread);
	if (philo)
		free(philo);
}

void	clear_mutex(t_data *data, t_fork *forks, int nb_threads)
{
	int	i;

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

int	panic(t_philo	*philo, pthread_t *threads, t_fork *fork, int index)
{
	int	i;

	i = 0;
	if (index >= 2)
	{
		while (i < philo->data->error_thread)
		{
			pthread_join(threads[i], NULL);
			i++;
		}
	}
	i = 0;
	if (index >= 1)
	{
		while (i < philo->data->error_mutex)
		{
			pthread_mutex_destroy(&fork[i].in_use);
			i++;
		}
		if (philo->data->error_mutex == philo->data->nb_philo)
			pthread_mutex_destroy(&philo->data->glob);
	}
	clean_process(fork, philo, threads);
	return (0);
}
