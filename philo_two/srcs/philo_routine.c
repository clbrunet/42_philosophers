/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clbrunet <clbrunet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 08:25:19 by clbrunet          #+#    #+#             */
/*   Updated: 2021/05/06 19:18:01 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

static void		take_forks(t_thread_arg *arg)
{
	if (sem_wait(arg->globs->forks_sem) || sem_wait(arg->globs->forks_sem))
		printf("Waiting for fork semaphore error\n");
	if (arg->globs->is_finished)
		return ;
	printf("%07lu %i %s\n", get_time_ms() - arg->globs->epoch, arg->nb,
			"has taken a fork");
	printf("%07lu %i %s\n", get_time_ms() - arg->globs->epoch, arg->nb,
			"has taken a fork");
}

static void		eat(t_thread_arg *arg, unsigned int *i)
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
		ft_usleep(arg->globs->time_to_eat);
	}
	(*i)++;
	if (arg->globs->should_stop
			&& *i == arg->globs->nb_time_philosophers_must_eat)
	{
		sem_wait(arg->globs->nb_done_sem);
		arg->globs->nb_done++;
		sem_post(arg->globs->nb_done_sem);
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
	ft_usleep(arg->globs->time_to_sleep);
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
		eat(arg, &i);
		sleep_routine(arg);
	}
	return (NULL);
}
