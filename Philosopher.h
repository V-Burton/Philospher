/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 10:51:57 by vburton           #+#    #+#             */
/*   Updated: 2023/04/04 19:03:01 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_philo t_philo;
typedef struct s_list t_list;

#define EMPTY 0
#define TAKEN 1

enum status
{
	is_dead,
	is_alive,
	is_thinking,
	is_sleeping,
	is_eating,
};

typedef struct s_philo
{
	struct timeval	timestamp;
	int				right_hand;
	int				left_hand;
	enum status		status;
}						t_philo;

typedef struct s_list
{
	t_philo philo;
	t_list *next;
}						t_list;

typedef struct s_data
{
	int nb_philo;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int time_to_think;
	int number_of_time_each_philosopher_must_eat;
}						t_data;

### Init ###
void	ft_fill_data(t_data *data, int argc, char **argv);