/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:46:13 by vburton           #+#    #+#             */
/*   Updated: 2023/04/28 14:39:48 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include "libft/libft.h"

# define EMPTY 0
# define FULL 1
# define AVAILABLE 0
# define UNAVAILABLE 1
# define USE 1
# define PAIR 0
# define ODD 1
# define NOT_THINKING 0
# define THINKING 1
# define HAS_EATEN 2

typedef struct s_philo t_philo;

typedef struct s_fork
{
	int status;
	pthread_mutex_t in_use;
}			t_fork;

typedef struct	s_data
{
	int			nb_philo;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	int			minimum_nb_of_time_each_philo_must_eat;
	int			running;
	pthread_mutex_t start;
	pthread_mutex_t death;
	pthread_mutex_t write;
	t_philo	*philosopher;
}			t_data;

struct s_philo
{
	int id;
	int	is_odd;
	int nb_meal;
	int status;
	int right_hand;
	int left_hand;
	long	actual_time;
	long 	time_last_meal;
	t_fork	*right_fork;
	t_fork	*left_fork;
	t_data	*data;
};

/*** utils ***/
long get_actual_time();
void	safe_printf(t_philo *philo, char *message);
int		is_odd(int i);

#endif