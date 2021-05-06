/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_process2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clbrunet <clbrunet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/05 21:14:16 by clbrunet          #+#    #+#             */
/*   Updated: 2021/05/06 14:50:37 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

static int	init_death_sem_error(t_globals *globs)
{
	sem_close(globs->forks_sem);
	sem_close(globs->nb_done_sem);
	sem_post(globs->is_finished_sem);
	sem_close(globs->is_finished_sem);
	sem_close(globs->output_sem);
	return (1);
}

int			init_death_sem(t_process_arg *arg)
{
	char			*i_str;
	char			*sem_name;

	i_str = ft_itoa(arg->nb);
	if (i_str == NULL)
		return (init_death_sem_error(arg->globs));
	sem_name = ft_strjoin(DEATH_SEM_NAME_START, i_str);
	free(i_str);
	if (sem_name == NULL)
		return (init_death_sem_error(arg->globs));
	sem_unlink(sem_name);
	arg->death_sem = sem_open(sem_name, O_CREAT | O_EXCL, S_IRWXU, 1);
	free(sem_name);
	if (arg->death_sem == SEM_FAILED)
		return (init_death_sem_error(arg->globs));
	return (0);
}

void		close_death_sem(t_process_arg *arg)
{
	char			*i_str;
	char			*sem_name;

	sem_close(arg->death_sem);
	i_str = ft_itoa(arg->nb);
	if (i_str == NULL)
		return ;
	sem_name = ft_strjoin(DEATH_SEM_NAME_START, i_str);
	free(i_str);
	if (sem_name == NULL)
		return ;
	sem_unlink(sem_name);
	free(sem_name);
}
