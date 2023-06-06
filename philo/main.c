/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <vburton@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:45:52 by vburton           #+#    #+#             */
/*   Updated: 2023/06/06 20:50:31 by vburton          ###   ########.fr       */
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
	usleep(100);
	routine_death(philo, &data);
	join_philo(threads, &data);
	clear_mutex(&data, fork, data.nb_philo);
	clean_process(fork, philo, threads);
	return (0);
}

void	clean_process(t_fork *fork, t_philo *philo, pthread_t *thread)
{
	free(fork);
	free(philo);
	free(thread);
}

void	fill_data(t_data *data, int argc, char **argv)
{
	data->nb_philo = ft_atoi(argv[1]);
	data->T2D = ft_atoi(argv[2]) * 1000;
	data->T2E = ft_atoi(argv[3]) * 1000;
	data->T2S = ft_atoi(argv[4]) * 1000;
	data->start_time = 0;
	if (argc == 6)
		data->min_nb_meal = ft_atoi(argv[5]);
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
		philo[i].right_fork = &fork[i];
		if (i == data->nb_philo - 1)
			philo[i].left_fork = &fork[0];
		else
			philo[i].left_fork = &fork[i + 1];
		i++;
	}
}

void	join_philo(pthread_t *threads, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

void	creat_philo(pthread_t *threads, t_data *data, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_create(&threads[i], NULL, routine, (void *)&philo[i]);
		i++;
	}
}

void	clear_mutex(t_data *data, t_fork *forks, int nb_threads)
{
	int	i;

	i = 0;
	while (i < nb_threads)
	{
		pthread_mutex_lock(&forks[i].in_use);
		pthread_mutex_unlock(&forks[i].in_use);
		i++;
	}
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

void	*routine(void *arg)
{
	time_t	delta;
	t_philo	*philo;

	philo = (t_philo *)arg;
	delta = philo->data->T2D - philo->data->T2S - philo->data->T2E;
	philo->time_last_meal = get_actual_time(philo->data->start_time) * 1000;
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
		if (philo->data->nb_philo == 1)
			return (0);
	}
}

void	pair_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_fork->in_use);
	safe_printf(philo, MSG_FORK);
	if (philo->data->nb_philo == 1)
	{
		pthread_mutex_unlock(&philo->left_fork->in_use);
		return ;
	}
	pthread_mutex_lock(&philo->right_fork->in_use);
	safe_printf(philo, MSG_FORK);
	safe_printf(philo, MSG_EAT);
	pthread_mutex_lock(&philo->data->glob);
	philo->time_last_meal = get_actual_time(philo->data->start_time) * 1000;
	pthread_mutex_unlock(&philo->data->glob);
	usleep(philo->data->T2E);
	pthread_mutex_unlock(&philo->left_fork->in_use);
	pthread_mutex_unlock(&philo->right_fork->in_use);
	pthread_mutex_lock(&philo->data->glob);
	philo->nb_meal++;
	pthread_mutex_unlock(&philo->data->glob);
	safe_printf(philo, MSG_SLEEP);
	usleep(philo->data->T2S);
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
	pthread_mutex_unlock(&philo->data->glob);
	usleep(philo->data->T2E);
	pthread_mutex_unlock(&philo->right_fork->in_use);
	pthread_mutex_unlock(&philo->left_fork->in_use);
	pthread_mutex_lock(&philo->data->glob);
	philo->nb_meal++;
	pthread_mutex_unlock(&philo->data->glob);
	safe_printf(philo, MSG_SLEEP);
	usleep(philo->data->T2S);
	safe_printf(philo, MSG_THINK);
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

int	check_full_n_death(t_philo	*philos, t_data *data, int i, int j)
{
	int	time;
	int	diff1;
	int	diff2;

	time = get_actual_time(philos->data->start_time) * 1000;
	pthread_mutex_lock(&philos->data->glob);
	diff1 = time - philos[i].time_last_meal;
	diff2 = time - philos[j].time_last_meal;
	if (check_full(philos, data) == 1)
		return (1);
	if (diff1 > data->T2D)
	{
		philos->data->run = 0;
		printf("%ld %d died\n", get_actual_time(philos->data->start_time), i);
		return (1);
	}
	if (diff2 > data->T2D)
	{
		philos->data->run = 0;
		printf("%ld %d died\n", get_actual_time(philos->data->start_time), j);
		return (1);
	}
	return (0);
}

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

int	check_full(t_philo	*philos, t_data *data)
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
