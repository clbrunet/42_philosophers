/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clbrunet <clbrunet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 16:08:06 by clbrunet          #+#    #+#             */
/*   Updated: 2021/05/04 16:08:32 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

static char		death_sems_malloc_error(t_globals *globs)
{
	sem_close(globs->forks_sem);
	sem_unlink(FORKS_SEM_NAME);
	sem_close(globs->nb_done_sem);
	sem_unlink(NB_DONE_SEM_NAME);
	return (1);
}

static char		death_sems_open_error(t_globals *globs,
		t_thread *threads, unsigned int i)
{
	char	*i_str;
	char	*sem_name;

	sem_close(globs->forks_sem);
	sem_unlink(FORKS_SEM_NAME);
	sem_close(globs->nb_done_sem);
	sem_unlink(NB_DONE_SEM_NAME);
	while (i != (unsigned)-1)
	{
		sem_close(threads[i].arg.death_sem);
		i_str = ft_itoa(i + 1);
		if (i_str == NULL)
			return (1);
		sem_name = ft_strjoin(DEATH_SEM_NAME_START, i_str);
		free(i_str);
		if (sem_name == NULL)
			return (1);
		sem_unlink(sem_name);
		free(sem_name);
		i--;
	}
	return (1);
}

char			initialize_threads_death_sems(t_globals *globs,
		t_thread *threads)
{
	unsigned int	i;
	char			*i_str;
	char			*sem_name;

	i = 0;
	while (i < globs->nb_philosopher)
	{
		i_str = ft_itoa(i + 1);
		if (i_str == NULL)
			return (death_sems_malloc_error(globs));
		sem_name = ft_strjoin(DEATH_SEM_NAME_START, i_str);
		free(i_str);
		if (sem_name == NULL)
			return (death_sems_malloc_error(globs));
		sem_unlink(sem_name);
		threads[i].arg.death_sem = sem_open(sem_name, O_CREAT | O_EXCL, S_IRWXU,
				1);
		free(sem_name);
		if (threads[i].arg.death_sem == SEM_FAILED)
			return (death_sems_open_error(globs, threads, i - 1));
		i++;
	}
	return (0);
}
