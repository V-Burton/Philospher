/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <vburton@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:46:13 by vburton           #+#    #+#             */
/*   Updated: 2023/06/12 18:22:53 by vburton          ###   ########.fr       */
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
	long			t2d;
	long			t2e;
	long			t2s;
	long			start_time;
	int				min_nb_meal;
	int				run;
	int				error_mutex;
	int				error_thread;
	pthread_mutex_t	glob;
	t_philo			*philosopher;
	t_fork			*forks;
}			t_data;

struct s_philo
{
	int		id;
	int		is_odd;
	int		nb_meal;
	int		status;
	long	actual_time;
	long	time_last_meal;
	t_fork	*right_fork;
	t_fork	*left_fork;
	t_data	*data;
};

/*** death_monitor ***/
int		routine_death(t_philo	*philos, t_data *data);

/*** init_philo ***/
int		init_mutex(t_data *data, t_fork *fork);
void	init_philo(t_data *data, t_philo *philo, t_fork *fork);
void	fill_data(t_data *data, int argc, char **argv, t_fork *forks);

/*** clean_philo ***/
void	join_philo(pthread_t *threads, t_data *data);
void	clean_process(t_fork *fork, t_philo *philo, pthread_t *thread);
void	clear_mutex(t_data *data, t_fork *forks, int nb_threads);

/*** utils ***/
long	get_actual_time(long start_time);
void	safe_printf(t_philo *philo, char *message);
int		is_odd(int i);
time_t	define_delta(long t2d, long t2s, long t2e);
int		panic(t_philo	*philo, pthread_t *threads, t_fork *fork, int index);

#endif
