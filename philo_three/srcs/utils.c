/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clbrunet <clbrunet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 18:55:20 by clbrunet          #+#    #+#             */
/*   Updated: 2021/05/06 19:16:33 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

static char		initialize_sems2(t_globals *globs)
{
	sem_unlink(OUTPUT_SEM_NAME);
	globs->output_sem = sem_open(OUTPUT_SEM_NAME, O_CREAT | O_EXCL,
			S_IRWXU, 1);
	if (globs->output_sem == SEM_FAILED)
	{
		sem_close(globs->forks_sem);
		sem_unlink(FORKS_SEM_NAME);
		sem_close(globs->nb_done_sem);
		sem_unlink(NB_DN_SEM_NAME);
		sem_close(globs->is_finished_sem);
		sem_unlink(IS_FINISHED_SEM_NAME);
		return (1);
	}
	return (0);
}

static char		initialize_sems(t_globals *globs)
{
	sem_unlink(FORKS_SEM_NAME);
	globs->forks_sem = sem_open(FORKS_SEM_NAME, O_CREAT | O_EXCL, S_IRWXU,
			globs->nb_philosopher);
	if (globs->forks_sem == SEM_FAILED)
		return (1);
	sem_unlink(NB_DN_SEM_NAME);
	globs->nb_done_sem = sem_open(NB_DN_SEM_NAME, O_CREAT | O_EXCL, S_IRWXU, 0);
	if (globs->nb_done_sem == SEM_FAILED)
	{
		sem_close(globs->forks_sem);
		sem_unlink(FORKS_SEM_NAME);
		return (1);
	}
	sem_unlink(IS_FINISHED_SEM_NAME);
	globs->is_finished_sem = sem_open(IS_FINISHED_SEM_NAME, O_CREAT | O_EXCL,
			S_IRWXU, 0);
	if (globs->is_finished_sem == SEM_FAILED)
	{
		sem_close(globs->forks_sem);
		sem_unlink(FORKS_SEM_NAME);
		sem_close(globs->nb_done_sem);
		sem_unlink(NB_DN_SEM_NAME);
		return (1);
	}
	return (initialize_sems2(globs));
}

char			initialize_simulation(t_globals *globs,
		t_process_arg *process_arg)
{
	if (initialize_sems(globs) != 0)
		return (1);
	process_arg->globs = globs;
	process_arg->death_time = globs->time_to_die;
	process_arg->is_finished = 0;
	return (0);
}

void			destroy_simulation(t_globals *globs)
{
	sem_close(globs->forks_sem);
	sem_unlink(FORKS_SEM_NAME);
	sem_close(globs->nb_done_sem);
	sem_unlink(NB_DN_SEM_NAME);
	sem_close(globs->is_finished_sem);
	sem_unlink(IS_FINISHED_SEM_NAME);
	sem_close(globs->output_sem);
	sem_unlink(OUTPUT_SEM_NAME);
}
