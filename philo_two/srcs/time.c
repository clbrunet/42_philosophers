/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clbrunet <clbrunet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 19:12:49 by clbrunet          #+#    #+#             */
/*   Updated: 2021/05/06 19:15:25 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

unsigned long	get_time_ms(void)
{
	struct timeval	timeval;

	if (gettimeofday(&timeval, NULL) != 0)
		return (-1);
	return (timeval.tv_sec * 1000 + timeval.tv_usec / 1000);
}


void			ft_usleep(unsigned long duration)
{
	unsigned long	start;
	unsigned long	current;

	start = get_time_ms();
	current = start;
	while (duration > current - start)
	{
		usleep(100);
		current = get_time_ms();
	}
}
