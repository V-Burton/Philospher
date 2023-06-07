/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <vburton@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 10:40:16 by vburton           #+#    #+#             */
/*   Updated: 2023/06/07 12:07:10 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	fill_data(t_data *data, int argc, char **argv)
{
	data->nb_philo = ft_atoi(argv[1]);
	data->t2d = ft_atoi(argv[2]) * 1000;
	data->t2e = ft_atoi(argv[3]) * 1000;
	data->t2s = ft_atoi(argv[4]) * 1000;
	data->start_time = 0;
	if (argc == 6)
		data->min_nb_meal = ft_atoi(argv[5]);
	else
		data->min_nb_meal = -1;
	data->run = 1;
}

void	init_mutex(t_data *data, t_fork *fork)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_init(&fork[i].in_use, NULL);
		i++;
	}
	pthread_mutex_init(&data->glob, NULL);
}

void	init_philo(t_data *data, t_philo *philo, t_fork *fork)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		philo[i].id = i + 1;
		philo[i].nb_meal = 0;
		philo[i].right_fork = &fork[i - 1];
		philo[i].is_odd = is_odd(i);
		philo[i].data = data;
		philo[i].time_last_meal = 0;
		philo[i].right_fork = &fork[i];
		if (i == data->nb_philo - 1)
			philo[i].left_fork = &fork[0];
		else
			philo[i].left_fork = &fork[i + 1];
		i++;
	}
}
