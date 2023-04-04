/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 13:59:12 by vburton           #+#    #+#             */
/*   Updated: 2023/04/04 15:52:17 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosopher.h"

void	*init_philo(struct timeval start_time);

void	creat_philo(t_list *list,int nb_philo, struct timeval start_time)
{
	int 	i;
	t_list	first;
	pthread_t *philo;

	i = 0;
	philo = malloc(sizeof(pthread_t) * nb_philo);
	while (i < nb_philo)
	{
		pthread_create(&philo[i], NULL, &init_philo, list->philo, start_time);
		list++;
		i++;
	}
	list->next = &first;
}

void	*init_philo(t_philo philo, struct timeval start_time)
{
	philo.timestamp = start_time;
	philo.left_hand = EMPTY;
	philo.right_hand = EMPTY;
	philo.status = is_alive;
}

void	ft_fill_data(t_data *data, int argc, char **argv)
{
	data->nb_philo = argv[1];
	data->time_to_die = argv[2];
	data->time_to_eat = argv[3];
	data->time_to_sleep = argv[4];
	if (argc == 6)
		data->number_of_time_each_philosopher_must_eat = argv[5];
	else
		data->number_of_time_each_philosopher_must_eat = -1;
	data->time
}