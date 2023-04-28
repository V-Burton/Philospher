/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:45:52 by vburton           #+#    #+#             */
/*   Updated: 2023/04/28 14:47:39 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	fill_data(t_data *data, int argc, char **argv);
void	init_philo(t_data *data, t_philo *philo, t_fork *fork);
void	init_mutex(t_data *data, t_fork *fork);
void	creat_philo(pthread_t *threads, t_data *data, t_philo *philo);
void	*routine(void *arg);
void	clear_mutex(t_data *data, t_fork *forks, int nb_threads);
int		check_death(t_philo *philo, long time_last_meal, long time_to_die);

int main(int argc, char **argv)
{
	t_fork	*fork;
	t_philo	*philo;
	t_data	data;
	pthread_t  *threads;

	if (argc < 5 || argc > 6)
		return (printf("wrong number of argument\n"));
	// Recuperer les arguments
	fill_data(&data, argc, argv);
	fork = malloc(sizeof(t_fork) * ft_atoi(argv[1]));
	philo = malloc(sizeof(t_philo) * ft_atoi(argv[1]));
	// Initialiser les mutex et les threads
	threads = malloc(sizeof(pthread_t) * data.nb_philo);
	init_mutex(&data, fork);
	init_philo(&data, philo, fork);
	// Créer et exécuter les threads pour chaque philosophe
	creat_philo(threads, &data, philo);
	// Attendre que les threads se terminent (ils ne devraient pas se terminer dans ce cas)
	// Nettoyer et détruire les mutex
	while(data.running) { ; }
	clear_mutex(&data, fork, data.nb_philo);
	return 0;
}


void	fill_data(t_data *data, int argc, char **argv)
{
	data->nb_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
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
		fork[i].status = AVAILABLE;
		pthread_mutex_init(&fork[i].in_use, NULL);
		i++;
	}
	pthread_mutex_init(&data->death, NULL);
	pthread_mutex_init(&data->start, NULL);
	pthread_mutex_init(&data->write, NULL);
}

void	init_philo(t_data *data, t_philo *philo, t_fork *fork)
{
	int i;

	i = 0;

	while(i < data->nb_philo)
	{
		philo[i].id = i;
		philo[i].status = NOT_THINKING;
		philo[i].nb_meal = 0;
		philo[i].right_hand = EMPTY;
		philo[i].left_hand = EMPTY;
		philo[i].time_last_meal = 0;
		philo[i].right_fork = &fork[i - 1];
		philo[i].is_odd = is_odd(i);
		philo[i].data = data;
		if (i == 0)
		{
			philo[i].right_fork = &fork[i];
		}
		if (i == data->nb_philo - 1)
		{
			philo[i].left_fork = &fork[0];
		}
		else
		{
			philo[i].left_fork = &fork[i + 1];
		}
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
	while (i < data->nb_philo)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
//	sleep(2);
}

void	*routine(void *arg)
{
	t_philo	*philo = (t_philo *)arg;

	while (1)
	{
		philo->data->running = check_death(philo, philo->time_last_meal, philo->data->time_to_die);
		if (philo->is_odd == PAIR)
		{
			if (philo->left_fork->status == AVAILABLE)
			{
				pthread_mutex_lock(&philo->left_fork->in_use);
				philo->left_fork->status = UNAVAILABLE;
				if (philo->right_fork->status == UNAVAILABLE)
				{
					pthread_mutex_lock(&philo->right_fork->in_use);
					philo->right_fork->status = UNAVAILABLE;
					if (philo->data->running == 0)
						break ;
					philo->nb_meal++;
					safe_printf(philo, "is eating");
					pthread_mutex_unlock(&philo->right_fork->in_use);
					philo->status = HAS_EATEN;
					philo->right_fork->status = AVAILABLE;
				}
				pthread_mutex_unlock(&philo->left_fork->in_use);
				philo->left_fork->status = AVAILABLE;
				if (philo->status == HAS_EATEN)
				{
					safe_printf(philo, "is sleeping");
					usleep(philo->data->time_to_sleep);
					philo->status = NOT_THINKING;
				}
			}
		}
		else
		{
			if (philo->right_fork->status == AVAILABLE)
			{
				pthread_mutex_lock(&philo->right_fork->in_use);
				philo->right_fork->status = UNAVAILABLE;
				if (philo->left_fork->status == UNAVAILABLE) {
					pthread_mutex_lock(&philo->left_fork->in_use);
					philo->left_fork->status = UNAVAILABLE;
					if (philo->data->running == 0)
						return (NULL);
					philo->nb_meal++;
					safe_printf(philo, "is eating.");
					pthread_mutex_unlock(&philo->left_fork->in_use);
					philo->status = HAS_EATEN;
					philo->left_fork->status = AVAILABLE;
				}
				pthread_mutex_unlock(&philo->right_fork->in_use);
				philo->right_fork->status = AVAILABLE;
				if (philo->status == HAS_EATEN)
				{
					safe_printf(philo, "is sleeping");
					usleep(philo->data->time_to_sleep);
					philo->status = NOT_THINKING;
				}
			}
			if (philo->status != THINKING)
			{
				safe_printf(philo, "is thinking");
				philo->status = THINKING;
			}
		}
	}
	return (NULL);
}

void	clear_mutex(t_data *data, t_fork *forks, int nb_threads)
{
	int i;

	i = 0;
	while (i < nb_threads)
	{
		pthread_mutex_destroy(&forks[i].in_use);
		i++;
	}
	pthread_mutex_destroy(&data->write);
	pthread_mutex_destroy(&data->death);
	pthread_mutex_destroy(&data->start);
}

int check_death(t_philo *philo, long time_last_meal, long time_to_die)
{
	long diff;

	pthread_mutex_lock(&philo->data->death);
	philo->actual_time = get_actual_time();
	diff = philo->actual_time - time_last_meal;
//	dprintf(1, "philo %d actual time = %ld et time of death = %ld, time of last meal = %ld et diff = %ld\n", philo->id, philo->actual_time, time_to_die, time_last_meal, diff);
	if (diff > time_to_die) {
		safe_printf(philo, "died.\n");
		return (0);
	}
	pthread_mutex_unlock(&philo->data->death);
	return (1);
}