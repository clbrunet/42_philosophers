/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clbrunet <clbrunet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 08:25:19 by clbrunet          #+#    #+#             */
/*   Updated: 2021/05/06 19:13:53 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

static void		take_forks(t_thread_arg *arg, char const *is_finished_p)
{
	if (pthread_mutex_lock(arg->right_fork_mutex)
			|| pthread_mutex_lock(&arg->left_fork_mutex))
		printf("Locking fork mutex error\n");
	if (*is_finished_p)
		return ;
	printf("%07lu %i %s\n", get_time_ms() - arg->globs->epoch, arg->nb,
			"has taken a fork");
	printf("%07lu %i %s\n", get_time_ms() - arg->globs->epoch, arg->nb,
			"has taken a fork");
}

static void		eat(t_thread_arg *arg, char const *is_finished_p, unsigned int *i)
{
	pthread_mutex_lock(&arg->death_mutex);
	arg->death_time = get_time_ms() - arg->globs->epoch
		+ arg->globs->time_to_die;
	pthread_mutex_unlock(&arg->death_mutex);
	if (!*is_finished_p)
	{
		printf("%07lu %i %s\n", get_time_ms() - arg->globs->epoch, arg->nb,
				"is eating");
		ft_usleep(arg->globs->time_to_eat);
	}
	(*i)++;
	if (arg->globs->should_stop
			&& *i == arg->globs->nb_time_philosophers_must_eat)
	{
		pthread_mutex_lock(arg->nb_done_mutex);
		(*arg->nb_done)++;
		pthread_mutex_unlock(arg->nb_done_mutex);
	}
	if (pthread_mutex_unlock(&arg->left_fork_mutex)
		|| pthread_mutex_unlock(arg->right_fork_mutex))
		printf("Unlocking fork mutex error\n");
}

static void		sleep_routine(t_thread_arg *arg, char const *is_finished_p)
{
	if (*is_finished_p)
		return ;
	printf("%07lu %i %s\n", get_time_ms() - arg->globs->epoch, arg->nb,
			"is sleeping");
	ft_usleep(arg->globs->time_to_sleep);
}

void			*philo_routine(void *void_arg)
{
	t_thread_arg	*arg;
	unsigned int	i;
	char const		*is_finished_p;

	arg = void_arg;
	is_finished_p = &arg->globs->is_finished;
	i = 0;
	while (!*is_finished_p)
	{
		printf("%07lu %i %s\n", get_time_ms() - arg->globs->epoch, arg->nb,
				"is thinking");
		take_forks(arg, is_finished_p);
		eat(arg, is_finished_p, &i);
		sleep_routine(arg, is_finished_p);
	}
	return (NULL);
}
