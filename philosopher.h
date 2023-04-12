/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:46:13 by vburton           #+#    #+#             */
/*   Updated: 2023/04/12 18:08:49 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include "libft/libft.h"

#	define ALIVE 1
#	define THINKING 2
#	define EATING 3
#	define DEAD -1
#	define HAS_FORK 1
#	define NO_FORK 0
#	define ODD 1
#	define PAIR 0
#	define AVAILABLE 1
#	define UNAVAILABLE 0

typedef struct	s_philo t_philo;

typedef struct	s_philo
{
	int id;
	long t_start;
	long t_now;
	long t_last_meal;
	int status;
	int fork;
	int left_hand;
	int right_hand;
	t_philo	*right_neighbor;
	t_philo	*left_neighbor;
	pthread_mutex_t mutex;
	pthread_mutex_t main_mutex;
}				t_philo;

typedef struct	s_data
{
	int		nb_philo;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int 	minimum_nb_of_time_each_philo_must_eat;
	t_philo	*philosopher;
}			t_data;

#endif