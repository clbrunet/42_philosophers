/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clbrunet <clbrunet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 08:25:19 by clbrunet          #+#    #+#             */
/*   Updated: 2021/05/06 15:01:18 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

static void	*check_death_routine(void *void_arg)
{
	t_process_arg	*arg;

	arg = (t_process_arg *)void_arg;
	while (!arg->is_finished)
	{
		sem_wait(arg->death_sem);
		if (arg->death_time <= get_time_ms() - arg->globs->epoch)
		{
			sem_wait(arg->globs->output_sem);
			if (!arg->is_finished)
				printf("%07lu %i %s\n", get_time_ms() - arg->globs->epoch,
					arg->nb, "died");
			arg->is_finished = 1;
			sem_post(arg->death_sem);
			sem_post(arg->globs->is_finished_sem);
			usleep(100);
			sem_post(arg->globs->output_sem);
			break ;
		}
		sem_post(arg->death_sem);
	}
	return (NULL);
}

static void	*check_end_routine(void *void_arg)
{
	t_process_arg	*arg;

	arg = (t_process_arg *)void_arg;
	sem_wait(arg->globs->is_finished_sem);
	arg->is_finished = 1;
	sem_post(arg->globs->is_finished_sem);
	return (NULL);
}

int			philo_process(t_process_arg *arg)
{
	pthread_t	check_death_th;
	pthread_t	check_end_th;

	if (init_death_sem(arg) != 0)
		return (1);
	if (pthread_create(&check_death_th, NULL, check_death_routine, arg) != 0)
		return (1);
	if (pthread_create(&check_end_th, NULL, check_end_routine, arg) != 0)
		return (1);
	philo_routine(arg);
	pthread_join(check_death_th, NULL);
	pthread_join(check_end_th, NULL);
	sem_post(arg->globs->nb_done_sem);
	sem_close(arg->globs->forks_sem);
	sem_close(arg->globs->is_finished_sem);
	sem_close(arg->globs->nb_done_sem);
	sem_close(arg->globs->output_sem);
	close_death_sem(arg);
	return (0);
}
