/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <vburton@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:45:52 by vburton           #+#    #+#             */
/*   Updated: 2023/06/06 18:42:11 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	fill_data(t_data *data, int argc, char **argv);
void	init_philo(t_data *data, t_philo *philo, t_fork *fork);
void	init_mutex(t_data *data, t_fork *fork);
void	creat_philo(pthread_t *threads, t_data *data, t_philo *philo);
void	*routine(void *arg);
void	clear_mutex(t_data *data, t_fork *forks, int nb_threads);
int		check_death(t_philo	*philos, long T2D,t_data *data);
void	join_philo(pthread_t *threads, t_data *data);
void	clean_process(t_fork *fork, t_philo *philo, pthread_t *thread);
int check_full(t_philo	*philos, t_data *data);


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
	data.start_time = get_actual_time(0);
	creat_philo(threads, &data, philo);
	check_death(philo, data.time_to_die, &data);
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
	data->start_time = 0;
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
		philo[i].id = i + 1;
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
		pthread_create(&threads[i], NULL, routine, (void *)&philo[i]);
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
	pthread_mutex_lock(&data->glob);
	while (i < nb_threads)
	{
		pthread_mutex_destroy(&forks[i].in_use);
		i++;
	}
	pthread_mutex_unlock(&data->glob);
	pthread_mutex_destroy(&data->glob);
	pthread_mutex_destroy(&data->death);
	pthread_mutex_destroy(&data->start);
}

void	*routine(void *arg)
{
	time_t	delta;
	t_philo	*philo = (t_philo *)arg;

	delta = philo->data->time_to_die - philo->data->time_to_sleep - philo->data->time_to_eat;
	philo->time_last_meal = get_actual_time(philo->data->start_time) * 1000;
	safe_printf(philo, MSG_THINK);
	if (philo->data->nb_philo == 1)
	{
		safe_printf(philo, MSG_FORK);
		return (0);
	}
	while (1)
	{
		if (philo->is_odd == PAIR)
			usleep(delta * 0.33);
		pthread_mutex_lock(&philo->data->glob);
		if (philo->data->running == 0 || philo->nb_meal == philo->data->minimum_nb_of_time_each_philo_must_eat)
		{
			pthread_mutex_unlock(&philo->data->glob);
			return(0) ;
		}
		pthread_mutex_unlock(&philo->data->glob);
		if (philo->is_odd == PAIR)
		{
			pthread_mutex_lock(&philo->left_fork->in_use);
			safe_printf(philo, MSG_FORK);
			pthread_mutex_lock(&philo->right_fork->in_use);
			safe_printf(philo, MSG_FORK);
			safe_printf(philo, MSG_EAT);
			pthread_mutex_lock(&philo->data->glob);
			philo->time_last_meal = get_actual_time(philo->data->start_time) * 1000;
			pthread_mutex_unlock(&philo->data->glob);
			usleep(philo->data->time_to_eat);
			pthread_mutex_unlock(&philo->left_fork->in_use);
			pthread_mutex_unlock(&philo->right_fork->in_use);
			pthread_mutex_lock(&philo->data->glob);
			philo->nb_meal++;
			pthread_mutex_unlock(&philo->data->glob);
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
			pthread_mutex_lock(&philo->data->glob);
			philo->time_last_meal = get_actual_time(philo->data->start_time) * 1000;
			pthread_mutex_unlock(&philo->data->glob);
			usleep(philo->data->time_to_eat);
			pthread_mutex_unlock(&philo->right_fork->in_use);
			pthread_mutex_unlock(&philo->left_fork->in_use);
			pthread_mutex_lock(&philo->data->glob);
			philo->nb_meal++;
			pthread_mutex_unlock(&philo->data->glob);
			safe_printf(philo, MSG_SLEEP);
			usleep(philo->data->time_to_sleep);
			safe_printf(philo, MSG_THINK);
		}
	}
}

int		check_death(t_philo	*philos, long T2D, t_data *data)
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
		if (j <= i)
		{
			i = 0;
			j = philos->data->nb_philo -1;
		}
		time = get_actual_time(philos->data->start_time) * 1000;
		pthread_mutex_lock(&philos->data->glob);
		diff1 = time - philos[i].time_last_meal;
		diff2 = time - philos[j].time_last_meal;
		if (check_full(philos, data))
			return (1);
		if (diff1 > T2D)
		{
			philos->data->running = 0;
			printf("%ld %d died\n",get_actual_time(philos->data->start_time), i);
			pthread_mutex_unlock(&philos->data->glob);
			return (1);
		}
		if (diff2 > T2D)
		{
			philos->data->running = 0;
			printf("%ld %d died\n",get_actual_time(philos->data->start_time), j);
			pthread_mutex_unlock(&philos->data->glob);
			return (1);
		}
		pthread_mutex_unlock(&philos->data->glob);
		i++;
		j--;
	}
}

int check_full(t_philo	*philos, t_data *data)
{
	int i;
	int j;
	int count;

	i = 0;
	j = data->nb_philo;
	count = 1;
	while (i < j)
	{
		if (philos[i].nb_meal == data->minimum_nb_of_time_each_philo_must_eat)
		{
			philos[i].status = FULL;
			count++;
		}
		if (philos[j].nb_meal == data->minimum_nb_of_time_each_philo_must_eat)
		{
			philos[j].status = FULL;
			count++;
		}
		i++;
		j--;
	}
	if (count == data->nb_philo)
	{
		philos->data->running = 0;
		pthread_mutex_unlock(&philos->data->glob);
		return (1);
	}
	return (0);
}


