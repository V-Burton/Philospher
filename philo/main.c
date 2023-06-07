/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <vburton@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:45:52 by vburton           #+#    #+#             */
/*   Updated: 2023/06/07 10:41:35 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	fill_data(t_data *data, int argc, char **argv);
void	init_philo(t_data *data, t_philo *philo, t_fork *fork);
void	init_mutex(t_data *data, t_fork *fork);
void	creat_philo(pthread_t *threads, t_data *data, t_philo *philo);
void	*routine(void *arg);
void	clear_mutex(t_data *data, t_fork *forks, int nb_threads);
int		routine_death(t_philo	*philos, t_data *data);
void	join_philo(pthread_t *threads, t_data *data);
void	clean_process(t_fork *fork, t_philo *philo, pthread_t *thread);
int		check_full(t_philo	*philos, t_data *data);
void	odd_routine(t_philo *philo);
void	pair_routine(t_philo *philo);
int		check_full_n_death(t_philo	*philos, t_data *data, int i, int j);

int	main(int argc, char **argv)
{
	t_fork		*fork;
	t_philo		*philo;
	t_data		data;
	pthread_t	*threads;

	if (argc < 5 || argc > 6)
		return (printf("wrong number of argument\n"));
	fill_data(&data, argc, argv);
	fork = malloc(sizeof(t_fork) * ft_atoi(argv[1]));
	philo = malloc(sizeof(t_philo) * ft_atoi(argv[1]));
	threads = malloc(sizeof(pthread_t) * data.nb_philo);
	init_mutex(&data, fork);
	init_philo(&data, philo, fork);
	data.start_time = get_actual_time(0);
	creat_philo(threads, &data, philo);
	routine_death(philo, &data);
	join_philo(threads, &data);
	clear_mutex(&data, fork, data.nb_philo);
	clean_process(fork, philo, threads);
	return (0);
}

void	creat_philo(pthread_t *threads, t_data *data, t_philo *philo)
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
		return ;
	}
	while (i < data->nb_philo)
	{
		pthread_create(&threads[i], NULL, routine, (void *)&philo[i]);
		i++;
	}
}

void	*routine(void *arg)
{
	time_t	delta;
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->data->glob);
	delta = philo->data->t2d - philo->data->t2s - philo->data->t2e;
	philo->time_last_meal = get_actual_time(philo->data->start_time) * 1000;
	pthread_mutex_unlock(&philo->data->glob);
	safe_printf(philo, MSG_THINK);
	while (1)
	{
		if (philo->is_odd == PAIR)
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

void	pair_routine(t_philo *philo)
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

void	odd_routine(t_philo *philo)
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
