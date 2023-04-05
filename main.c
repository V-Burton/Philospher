/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 10:51:52 by vburton           #+#    #+#             */
/*   Updated: 2023/04/05 16:15:20 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosopher.h"

void	*routine()
{
	printf("test from threads\n");
}

int	main(int argc, char **argv)
{
	pthread_t 		t1;
	t_data			data;
	struct timeval	start_time;

	gettimeofday(&start_time, NULL);
	ft_fill_data(&data, argc, argv, start_time);
	pthread_create(&t1, NULL, &routine, NULL);
	pthread_join(t1, NULL);

	return (0);
}