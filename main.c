/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:45:52 by vburton           #+#    #+#             */
/*   Updated: 2023/04/27 19:53:27 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	fill_data(t_data *data, int argc, char **argv);
int init_philo(t_data data, t_philo *philo, t_fork *fork);
int init_mutex(t_data *data, t_fork *fork);
void	creat_philo(t_data *data, t_philo *philo);
void	*routine(void *arg);

int main(int argc, char **argv)
{
	t_fork	*fork;
	t_philo	*philo;
	t_data	data;

	if (argc < 5 || argc > 6)
		return (printf("wrong number of argument\n"));
	// Recuperer les arguments
	fill_data(&data, argc, argv);
	fork = malloc(sizeof(t_fork) * ft_atoi(argv[1]));
	philo = malloc(sizeof(t_philo) * ft_atoi(argv[1]));
	// Initialiser les mutex et les threads
	init_mutex(&data, fork);
	init_philo(data, philo, fork);
	// Créer et exécuter les threads pour chaque philosophe
	creat_philo(&data, philo);
	// Attendre que les threads se terminent (ils ne devraient pas se terminer dans ce cas)

	// Nettoyer et détruire les mutex
	// ...

	return 0;
}


int	fill_data(t_data *data, int argc, char **argv)
{
	data->nb_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->minimum_nb_of_time_each_philo_must_eat = ft_atoi(argv[5]);
}

int init_mutex(t_data *data, t_fork *fork)
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

int init_philo(t_data data, t_philo *philo, t_fork *fork)
{
	int i;

	i = 0;
	while(i < data.nb_philo)
	{
		philo[i].id = i;
		philo[i].nb_meal = 0;
		philo[i].right_hand = EMPTY;
		philo[i].left_hand = EMPTY;
		philo[i].time_last_meal = 0;
		philo[i].right_neighbor = &philo[i - 1];
		philo[i].right_fork = &fork[i - 1];
		philo[i].is_odd = is_odd(i);
		if (i == 0)
		{
			philo[i].right_fork = &fork[data.nb_philo - 1];
			philo[i].right_neighbor = &philo[data.nb_philo - 1];
		}
		if (i == data.nb_philo - 1)
		{
			philo[i].left_neighbor = &philo[0];
			philo[i].left_fork = &fork[0];
		}
		else
		{
			philo[i].left_neighbor = &philo[i + 1];
			philo[i].left_fork = &fork[1];
		}
		philo[i].data = &data;
		i++;
	}
}

void	creat_philo(t_data *data, t_philo *philo)
{
	int i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_create(&philo[i], NULL, routine, (void *)&philo[i]);
		i++;
	}
	while (i < data->nb_philo)
	{
		pthread_join();
		i++;
	}
}

void	*routine(void *arg)
{
	t_philo	*philo = (t_philo *)arg;

	while (1)
	{
		safe_printf(philo, "is thinking\n");
		if (philo->is_odd == PAIR)
		{
			if (philo->left_fork->status == AVAILABLE)
			{
				
			}
		}
	}

}
