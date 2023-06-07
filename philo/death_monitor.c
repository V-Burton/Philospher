/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <vburton@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 10:32:52 by vburton           #+#    #+#             */
/*   Updated: 2023/06/07 10:52:02 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	update_count(t_philo *philo1, t_philo *philo2, int count, int nb_meal)
{
	if (philo1->nb_meal >= nb_meal)
	{
		philo1->status = FULL;
		count += 1;
	}
	if (philo2->nb_meal >= nb_meal)
	{
		philo2->status = FULL;
		count += 1;
	}
	return (count);
}

static int	check_full(t_philo	*philos, t_data *data)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	j = data->nb_philo - 1;
	count = 1;
	while (i < j)
	{
		count = update_count(&philos[i], &philos[j], count, data->min_nb_meal);
		i++;
		j--;
	}
	if (count == data->nb_philo)
	{
		philos->data->run = 0;
		return (1);
	}
	return (0);
}

static int	check_full_n_death(t_philo	*philos, t_data *data, int i, int j)
{
	int	time;
	int	diff1;
	int	diff2;

	pthread_mutex_lock(&philos->data->glob);
	time = get_actual_time(philos->data->start_time) * 1000;
	diff1 = time - philos[i].time_last_meal;
	diff2 = time - philos[j].time_last_meal;
	if (check_full(philos, data) == 1)
		return (1);
	if (diff1 > data->t2d)
	{
		philos->data->run = 0;
		printf("%ld %d died\n", get_actual_time(data->start_time), i + 1);
		return (1);
	}
	if (diff2 > data->t2d)
	{
		philos->data->run = 0;
		printf("%ld %d died\n", get_actual_time(philos->data->start_time), j);
		return (1);
	}
	return (0);
}

int	routine_death(t_philo	*philos, t_data *data)
{
	int	i;
	int	j;

	i = 0;
	j = philos->data->nb_philo - 1;
	while (1)
	{
		if (j <= i)
		{
			i = 0;
			j = philos->data->nb_philo -1;
		}
		if (check_full_n_death(philos, data, i, j))
		{
			pthread_mutex_unlock(&philos->data->glob);
			return (1);
		}
		pthread_mutex_unlock(&philos->data->glob);
		i++;
		j--;
	}
}
