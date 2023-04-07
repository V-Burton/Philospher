/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:45:52 by vburton           #+#    #+#             */
/*   Updated: 2023/04/07 17:23:18 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	fill_data(t_data *data, int argc, char **argv);
t_philo	*init_philo(t_philo *philo, int nb_philo);
void	start_threads(t_philo *philo, int nb_philo);
long	get_actual_time();

int	main(int argc, char **argv)
{
	t_data data;

	fill_data(&data, argc, argv);
	data.philosopher = init_philo(data.philosopher, data.nb_philo);
	start_threads(data.philosopher, data.nb_philo);

	return (0);
}

void	fill_data(t_data *data, int argc, char **argv)
{
	data->nb_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->minimum_nb_of_time_each_philo_must_eat = ft_atoi(argv[5]);
	else
		data->minimum_nb_of_time_each_philo_must_eat = 0;
}

t_philo	*init_philo(t_philo *philo, int nb_philo)
{
	int i;
	struct timeval tv;
	long start_t;

	i = 0;
	philo = malloc(sizeof(t_philo) * nb_philo);
	gettimeofday(&tv, NULL);
	start_t = tv.tv_sec * 1000000 + tv.tv_usec;
	while (i < nb_philo)
	{
		philo[i].id = i;
		philo[i].status = ALIVE;
		philo[i].left_hand = NO_FORK;
		philo[i].right_hand = NO_FORK;
		philo[i].t_start = start_t;
		i++;
	}
	return (philo);
}

void	start_threads(t_philo *philo, int nb_philo)
{
	int i;
	int thread;

	i = 0;
	while (i < nb_philo)
	{
		philo[i].t_last_meal = get_actual_time();
		thread = pthread_create(&thread, NULL, &execution, )
		i++;
	}
}

long get_actual_time()
{
	struct	timeval tv;
	long	time;

	gettimeofday(&tv, NULL);
	time = tv.tv_sec * 1000000 + tv.tv_usec;
	return (time);
}