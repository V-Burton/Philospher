/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:45:52 by vburton           #+#    #+#             */
/*   Updated: 2023/04/12 18:25:07 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	fill_data(t_data *data, int argc, char **argv);
t_philo	*init_philo(t_philo *philo, int nb_philo);
void	start_threads(t_philo *philo, int nb_philo);
long	get_actual_time();
void	*routine(void *philo);
int	is_odd(int id);
int	check_available_fork(t_philo *actual, t_philo *neighbor);

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
		if (i == 0)
		{
			philo[nb_philo].id = nb_philo - 1;
			philo[i].right_neighbor = &philo[nb_philo];
		}
		else
			philo[i].right_neighbor = &philo[i - 1];
		if (i < nb_philo - 1)
			philo[i].left_neighbor = &philo[i + 1];
		else
			philo[i].left_neighbor = &philo[0];
		i++;
	}
	return (philo);
}

void	start_threads(t_philo *philo, int nb_philo)
{
	int i;
	int j;
	pthread_t thread[nb_philo];
	pthread_mutex_t mutex_timing;
	pthread_mutex_t mutex[nb_philo];

	pthread_mutex_init(&mutex_timing, NULL);
	i = 0;
	j = 1;
	while (i < nb_philo)
	{
		j = 1;
		philo[i].t_last_meal = get_actual_time();
		philo[i].main_mutex = mutex_timing;
		pthread_mutex_init(&philo[i].mutex, NULL);
		j = pthread_create(&thread[i], NULL, &routine, (void*)&philo[i]);
		i++;
		if (j == 0)
			pthread_mutex_unlock(&mutex_timing);
	}
	i = 0;
	while(i < nb_philo)
	{
		pthread_join(thread[i], NULL);
		i++;
	}
	pthread_mutex_destroy(mutex);
}


long get_actual_time()
{
	struct	timeval tv;
	long	time;

	gettimeofday(&tv, NULL);
	time = tv.tv_sec * 1000000 + tv.tv_usec;
	return (time);
}

void	*routine(void *philo)
{
	int i = 0;
	t_philo *philo_s;

	philo_s = (t_philo *)philo;
	while (i < 10) {
		if (is_odd(philo_s->id) == ODD)
			check_available_fork(philo_s, philo_s->right_neighbor);
		else
			check_available_fork(philo_s, philo_s->left_neighbor);
		i++;
	}
		return (0);
}

int	is_odd(int id)
{
	if (id % 2 == 0)
		return (PAIR);
	return (ODD);
}

int	check_available_fork(t_philo *actual, t_philo *neighbor)
{
	if (actual->fork == AVAILABLE && neighbor->status != EATING && neighbor->fork == AVAILABLE)
	{
		actual->status = EATING;
		actual->fork = UNAVAILABLE;
		neighbor->fork = UNAVAILABLE;
		printf("philo id : %d is eating\n", actual->id);
		printf("philo id : %d is waiting\n", neighbor->id);
		actual->t_last_meal = get_actual_time();
	}
	actual->fork = AVAILABLE;
	neighbor->fork = AVAILABLE;
}