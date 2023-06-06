/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <vburton@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:46:13 by vburton           #+#    #+#             */
/*   Updated: 2023/06/06 20:19:48 by vburton          ###   ########.fr       */
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
# define USE 1
# define PAIR 0
# define ODD 1
# define NOT_THINKING 0
# define THINKING 1
# define HAS_EATEN 2
# define MSG_EAT "is eating"
# define MSG_SLEEP "is sleeping"
# define MSG_THINK "is thinking"
# define MSG_FORK "has taken a fork"
# define MSG_DEAD "died"

typedef struct s_philo	t_philo;

typedef struct s_fork
{
	pthread_mutex_t	in_use;
}			t_fork;

typedef struct s_data
{
	int				nb_philo;
	long			T2D;
	long			T2E;
	long			T2S;
	long 			start_time;
	int				min_nb_meal;
	int				run;
	pthread_mutex_t	glob;
	t_philo			*philosopher;
}			t_data;

struct s_philo
{
	int		id;
	int		is_odd;
	int		nb_meal;
	int		status;
	long	actual_time;
	long 	time_last_meal;
	t_fork	*right_fork;
	t_fork	*left_fork;
	t_data	*data;
};

/*** utils ***/
long	get_actual_time(long start_time);
void	safe_printf(t_philo *philo, char *message);
int		is_odd(int i);

#endif
