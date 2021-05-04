/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clbrunet <clbrunet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 08:25:19 by clbrunet          #+#    #+#             */
/*   Updated: 2021/05/04 16:02:56 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

static void		take_forks(t_thread_arg *arg)
{
	sem_wait(arg->globs->forks_sem);
	if (!arg->globs->is_finished)
	{
		printf("%07lu %i %s\n", get_time_ms() - arg->globs->epoch, arg->nb,
				"has taken a fork");
	}
	sem_wait(arg->globs->forks_sem);
	if (arg->globs->is_finished)
		return ;
	printf("%07lu %i %s\n", get_time_ms() - arg->globs->epoch, arg->nb,
			"has taken a fork");
}

static void		eat(t_thread_arg *arg)
{
	take_forks(arg);
	sem_wait(arg->death_sem);
	arg->death_time = get_time_ms() - arg->globs->epoch
		+ arg->globs->time_to_die;
	sem_post(arg->death_sem);
	if (!arg->globs->is_finished)
	{
		printf("%07lu %i %s\n", get_time_ms() - arg->globs->epoch, arg->nb,
				"is eating");
		usleep(arg->globs->time_to_eat * 1000);
	}
	sem_post(arg->globs->forks_sem);
	sem_post(arg->globs->forks_sem);
}

static void		sleep_routine(t_thread_arg *arg)
{
	if (arg->globs->is_finished)
		return ;
	printf("%07lu %i %s\n", get_time_ms() - arg->globs->epoch, arg->nb,
			"is sleeping");
	usleep(arg->globs->time_to_sleep * 1000);
}

void			*philo_routine(void *void_arg)
{
	t_thread_arg	*arg;
	unsigned int	i;

	arg = void_arg;
	i = 0;
	while (!arg->globs->is_finished)
	{
		printf("%07lu %i %s\n", get_time_ms() - arg->globs->epoch, arg->nb,
				"is thinking");
		eat(arg);
		i++;
		if (arg->globs->should_stop
				&& i == arg->globs->nb_time_philosophers_must_eat)
		{
			sem_wait(arg->globs->nb_done_sem);
			arg->globs->nb_done++;
			sem_post(arg->globs->nb_done_sem);
		}
		sleep_routine(arg);
	}
	return (NULL);
}
