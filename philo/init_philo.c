/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <vburton@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 10:40:16 by vburton           #+#    #+#             */
/*   Updated: 2023/07/13 14:23:43 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	fill_data(t_data *data, int argc, char **argv, t_fork *fork)
{
	data->nb_philo = ft_atoi(argv[1]);
	data->t2d = ft_atoi(argv[2]) * 1000;
	data->t2e = ft_atoi(argv[3]) * 1000;
	data->t2s = ft_atoi(argv[4]) * 1000;
	data->start_time = 0;
	data->forks = fork;
	if (argc == 6)
		data->min_nb_meal = ft_atoi(argv[5]);
	else
		data->min_nb_meal = -1;
	data->run = 1;
	data->error_mutex = data->nb_philo;
	data->error_thread = data->nb_philo;
	if (data->nb_philo <= 0 || data->t2d <= 0 || data->t2e <= 0 || \
						data->t2s <= 0 || (argc == 6 && data->min_nb_meal <= 0))
	{
		printf("Invalid parameter\n");
		return (1);
	}
	return (0);
}

int	init_mutex(t_data *data, t_fork *fork)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&fork[i].in_use, NULL) != 0)
			return (data->error_mutex = i, 0);
		i++;
	}
	if (pthread_mutex_init(&data->glob, NULL) != 0)
		return (data->error_mutex = i + 1, 0);
	return (1);
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
		philo[i].is_odd = is_odd(i + 1);
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
