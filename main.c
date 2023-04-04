/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 10:51:52 by vburton           #+#    #+#             */
/*   Updated: 2023/04/04 13:56:51 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosopher.h"

void	*routine()
{
	printf("test from threads\n");
}

int	main(int argc, char **argv)
{
	pthread_t t1;
	t_data	data;

	ft_fill_data(&data, argc, argv);
	pthread_create(&t1, NULL, &routine, NULL);
	pthread_join(t1, NULL);

	return (0);
}