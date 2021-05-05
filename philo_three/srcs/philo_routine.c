/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clbrunet <clbrunet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/05 19:24:58 by clbrunet          #+#    #+#             */
/*   Updated: 2021/05/05 20:08:35 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

static void	take_forks(t_process_arg *arg)
{
	if (!arg->is_finished)
		sem_wait(arg->globs->forks_sem);
	if (!arg->is_finished)
		printf("%07lu %i %s\n", get_time_ms() - arg->globs->epoch, arg->nb,
			"has taken a fork");
	if (!arg->is_finished)
		sem_wait(arg->globs->forks_sem);
	if (!arg->is_finished)
		printf("%07lu %i %s\n", get_time_ms() - arg->globs->epoch, arg->nb,
			"has taken a fork");
}

static void	eat(t_process_arg *arg)
{
	take_forks(arg);
	sem_wait(arg->death_sem);
	arg->death_time = get_time_ms() - arg->globs->epoch
		+ arg->globs->time_to_die;
	sem_post(arg->death_sem);
	if (!arg->is_finished)
	{
		printf("%07lu %i %s\n", get_time_ms() - arg->globs->epoch, arg->nb,
			"is eating");
		usleep(arg->globs->time_to_eat * 1000);
	}
	sem_post(arg->globs->forks_sem);
	sem_post(arg->globs->forks_sem);
}

static void	sleep_routine(t_process_arg *arg)
{
	if (!arg->is_finished)
	{
		printf("%07lu %i %s\n", get_time_ms() - arg->globs->epoch, arg->nb,
			"is sleeping");
		usleep(arg->globs->time_to_sleep * 1000);
	}
}

int			philo_routine(t_process_arg *arg)
{
	unsigned int	i;

	i = 0;
	while (!arg->is_finished)
	{
		printf("%07lu %i %s\n", get_time_ms() - arg->globs->epoch, arg->nb,
				"is thinking");
		eat(arg);
		i++;
		if (arg->globs->should_stop
				&& i == arg->globs->nb_time_philosophers_must_eat)
			sem_post(arg->globs->nb_done_sem);
		sleep_routine(arg);
	}
	return (0);
}
