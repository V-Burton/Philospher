/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 13:59:12 by vburton           #+#    #+#             */
/*   Updated: 2023/04/05 18:17:25 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosopher.h"

void	*init_philo(struct timeval start_time);

void	creat_philo(t_data data)
{
	int 	i;
	t_list	first;
	t_list	list;
	pthread_t *philo;

	i = 1;
	philo = malloc(sizeof(pthread_t) * nb_philo);
	pthread_create(&philo[i], NULL, &init_philo, &data);
	first.philo = philo[0];
	list = first;
	while (i < nb_philo)
	{
		pthread_create(&philo[i], NULL, &init_philo, &data);
		list.philo = philo[i];
		list.next = list++;
		i++;
	}
	list->next = &first;
}

void	*init_philo(void *data)
{
	t_philo philo;

	philo = (t_data)data->philos.philo;
	philo.timestamp = (long)start_time;
	philo.left_hand = EMPTY;
	philo.right_hand = EMPTY;
	philo.status = is_alive;
}

void	ft_fill_data(t_data *data, int argc, char **argv, struct timeval start_time)
{
	t_list	*philos;

	data->start_time = start_time.tv_usec;
	data->philos = philos;
	data->nb_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->number_of_time_each_philosopher_must_eat = argv[5];
	else
		data->number_of_time_each_philosopher_must_eat = -1;
}