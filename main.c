/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <vburton@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:45:52 by vburton           #+#    #+#             */
/*   Updated: 2023/06/05 19:00:59 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	fill_data(t_data *data, int argc, char **argv);
void	init_philo(t_data *data, t_philo *philo, t_fork *fork);
void	init_mutex(t_data *data, t_fork *fork);
void	creat_philo(pthread_t *threads, t_data *data, t_philo *philo);
void	*routine(void *arg);
void	clear_mutex(t_data *data, t_fork *forks, int nb_threads);
int		check_death(t_philo	*philos, long T2D);
void	join_philo(pthread_t *threads, t_data *data);
void	clean_process(t_fork *fork, t_philo *philo, pthread_t *thread);


int main(int argc, char **argv)
{
	t_fork		*fork;
	t_philo		*philo;
	t_data		data;
	pthread_t 	*threads;

	if (argc < 5 || argc > 6)
		return (printf("wrong number of argument\n"));
	fill_data(&data, argc, argv);
	fork = malloc(sizeof(t_fork) * ft_atoi(argv[1]));
	philo = malloc(sizeof(t_philo) * ft_atoi(argv[1]));
	threads = malloc(sizeof(pthread_t) * data.nb_philo);
	init_mutex(&data, fork);
	init_philo(&data, philo, fork);
	creat_philo(threads, &data, philo);
	check_death(philo, data.time_to_die);
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
	data->time_to_die = ft_atoi(argv[2]) * 1000;
	data->time_to_eat = ft_atoi(argv[3]) * 1000;
	data->time_to_sleep = ft_atoi(argv[4]) * 1000;
	if (argc == 6)
		data->minimum_nb_of_time_each_philo_must_eat = ft_atoi(argv[5]);
	data->running = 1;
}

void	init_mutex(t_data *data, t_fork *fork)
{
	int i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_init(&fork[i].in_use, NULL);
		i++;
	}
	pthread_mutex_init(&data->death, NULL);
	pthread_mutex_init(&data->start, NULL);
	pthread_mutex_init(&data->glob, NULL);
}

void	init_philo(t_data *data, t_philo *philo, t_fork *fork)
{
	int	i;

	i = 0;

	while (i < data->nb_philo)
	{
		philo[i].id = i;
		philo[i].status = NOT_THINKING;
		philo[i].nb_meal = 0;
		philo[i].right_hand = EMPTY;
		philo[i].left_hand = EMPTY;
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
	int i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_create(threads, NULL, routine, (void *)&philo[i]);
		i++;
	}
}

void	clear_mutex(t_data *data, t_fork *forks, int nb_threads)
{
	int i;

	i = 0;
	while (i < nb_threads)
	{
		pthread_mutex_lock(&forks[i].in_use);
		pthread_mutex_unlock(&forks[i].in_use);
		i++;
	}
	i = 0;
	while (i < nb_threads)
	{
		pthread_mutex_destroy(&forks[i].in_use);
		i++;
	}
	pthread_mutex_destroy(&data->glob);
	pthread_mutex_destroy(&data->death);
	pthread_mutex_destroy(&data->start);
}

void	*routine(void *arg)
{
	t_philo	*philo = (t_philo *)arg;

	philo->time_last_meal = get_actual_time() * 1000;
	safe_printf(philo, MSG_THINK);
	while (1)
	{
		if (philo->data->running == 0 || philo->nb_meal == philo->data->minimum_nb_of_time_each_philo_must_eat)
			return(0) ;
		if (philo->is_odd == PAIR)
		{
			pthread_mutex_lock(&philo->left_fork->in_use);
			safe_printf(philo, MSG_FORK);
			pthread_mutex_lock(&philo->right_fork->in_use);
			safe_printf(philo, MSG_FORK);
			safe_printf(philo, MSG_EAT);
			philo->time_last_meal = get_actual_time() * 1000;
			usleep(philo->data->time_to_eat);
			pthread_mutex_unlock(&philo->left_fork->in_use);
			pthread_mutex_unlock(&philo->right_fork->in_use);
			philo->nb_meal++;
			safe_printf(philo, MSG_SLEEP);
			usleep(philo->data->time_to_sleep);
			safe_printf(philo, MSG_THINK);
		}
		else
		{
			pthread_mutex_lock(&philo->right_fork->in_use);
			safe_printf(philo, MSG_FORK);
			pthread_mutex_lock(&philo->left_fork->in_use);
			safe_printf(philo, MSG_FORK);
			safe_printf(philo, MSG_EAT);
			philo->time_last_meal = get_actual_time() * 1000;
			usleep(philo->data->time_to_eat);
			pthread_mutex_unlock(&philo->right_fork->in_use);
			pthread_mutex_unlock(&philo->left_fork->in_use);
			philo->nb_meal++;
			safe_printf(philo, MSG_SLEEP);
			usleep(philo->data->time_to_sleep);
			safe_printf(philo, MSG_THINK);
		}
	}
}

int		check_death(t_philo	*philos, long T2D)
{
	int	i;
	int	j;
	int	time;
	int	diff1;
	int	diff2;

	i = 0;
	j = philos->data->nb_philo - 1;
	while (1)
	{
		if (j < i)
		{
			i = 0;
			j = philos->data->nb_philo -1;
		}
		time = get_actual_time() * 1000;
		diff1 = time - philos[i].time_last_meal;
		diff2 = time - philos[j].time_last_meal;
		if (philos[i].is_odd == ODD && philos[i].nb_meal == \
			philos->data->minimum_nb_of_time_each_philo_must_eat)
		{
			philos->data->running = 0;
			return (1);
		}
		if (diff1 > T2D)
		{
			philos->data->running = 0;
			pthread_mutex_lock(&philos->data->glob);
			printf("%ld %d died\n",get_actual_time(), i);
			pthread_mutex_unlock(&philos->data->glob);
			return (1);
		}
		if (diff2 > T2D)
		{
			philos->data->running = 0;
			pthread_mutex_lock(&philos->data->glob);
			printf("%ld %d died\n",get_actual_time(), j);
			pthread_mutex_unlock(&philos->data->glob);
			return (1);
		}
		i++;
		j--;
	}
}

