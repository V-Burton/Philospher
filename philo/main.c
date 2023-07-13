/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <vburton@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:45:52 by vburton           #+#    #+#             */
/*   Updated: 2023/07/13 14:27:54 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static void	pair_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_fork->in_use);
	safe_printf(philo, MSG_FORK);
	pthread_mutex_lock(&philo->right_fork->in_use);
	safe_printf(philo, MSG_FORK);
	safe_printf(philo, MSG_EAT);
	pthread_mutex_lock(&philo->data->glob);
	philo->time_last_meal = get_actual_time(philo->data->start_time) * 1000;
	philo->nb_meal++;
	pthread_mutex_unlock(&philo->data->glob);
	usleep(philo->data->t2e);
	pthread_mutex_unlock(&philo->left_fork->in_use);
	pthread_mutex_unlock(&philo->right_fork->in_use);
	safe_printf(philo, MSG_SLEEP);
	usleep(philo->data->t2s);
	safe_printf(philo, MSG_THINK);
}

static void	odd_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->right_fork->in_use);
	safe_printf(philo, MSG_FORK);
	pthread_mutex_lock(&philo->left_fork->in_use);
	safe_printf(philo, MSG_FORK);
	safe_printf(philo, MSG_EAT);
	pthread_mutex_lock(&philo->data->glob);
	philo->time_last_meal = get_actual_time(philo->data->start_time) * 1000;
	philo->nb_meal++;
	pthread_mutex_unlock(&philo->data->glob);
	usleep(philo->data->t2e);
	pthread_mutex_unlock(&philo->right_fork->in_use);
	pthread_mutex_unlock(&philo->left_fork->in_use);
	safe_printf(philo, MSG_SLEEP);
	usleep(philo->data->t2s);
	safe_printf(philo, MSG_THINK);
}

static void	*routine(void *arg)
{
	time_t	delta;
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->data->glob);
	delta = define_delta(philo->data->t2d, philo->data->t2s, philo->data->t2e);
	philo->time_last_meal = get_actual_time(philo->data->start_time) * 1000;
	pthread_mutex_unlock(&philo->data->glob);
	safe_printf(philo, MSG_THINK);
	while (1)
	{
		if (philo->is_odd == ODD)
			usleep(delta * 0.33);
		pthread_mutex_lock(&philo->data->glob);
		if (philo->data->run == 0 || philo->nb_meal == philo->data->min_nb_meal)
		{
			pthread_mutex_unlock(&philo->data->glob);
			return (0);
		}
		pthread_mutex_unlock(&philo->data->glob);
		if (philo->is_odd == PAIR)
			pair_routine(philo);
		else
			odd_routine(philo);
	}
}

static int	creat_philo(pthread_t *threads, t_data *data, t_philo *philo)
{
	int	i;

	i = 0;
	if (data->nb_philo == 1)
	{
		pthread_mutex_lock(&philo->data->glob);
		printf("%ld %d %s\n", get_actual_time(data->start_time), 1, MSG_THINK);
		printf("%ld %d %s\n", get_actual_time(data->start_time), 1, MSG_FORK);
		usleep(data->t2d);
		printf("%ld %d died\n", get_actual_time(data->start_time), 1);
		pthread_mutex_unlock(&philo->data->glob);
		return (1);
	}
	pthread_mutex_lock(&philo->data->glob);
	while (i < data->nb_philo)
	{
		if (pthread_create(&threads[i], NULL, routine, (void *)&philo[i]) != 0)
			return (pthread_mutex_unlock(&philo->data->glob), \
								data->run = 0, data->error_thread = i, 0);
		i++;
	}
	pthread_mutex_unlock(&philo->data->glob);
	return (1);
}

int	main(int argc, char **argv)
{
	t_fork		*fork;
	t_philo		*philo;
	t_data		data;
	pthread_t	*threads;

	if (argc < 5 || argc > 6)
		return (printf("wrong number of argument\n"));
	fork = malloc(sizeof(t_fork) * ft_atoi(argv[1]));
	philo = malloc(sizeof(t_philo) * ft_atoi(argv[1]));
	threads = malloc(sizeof(pthread_t) * ft_atoi(argv[1]));
	if (!threads || !philo || !fork || fill_data(&data, argc, argv, fork))
		return (panic(philo, threads, fork, 0));
	init_philo(&data, philo, fork);
	if (!init_mutex(&data, fork))
		return (panic(philo, threads, fork, 1));
	data.start_time = get_actual_time(0);
	if (!creat_philo(threads, &data, philo))
		return (panic(philo, threads, fork, 2));
	if (data.nb_philo > 1)
		routine_death(philo, &data);
	join_philo(threads, &data);
	clear_mutex(&data, fork, data.nb_philo);
	clean_process(fork, philo, threads);
	return (0);
}
