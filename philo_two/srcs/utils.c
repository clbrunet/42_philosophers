/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clbrunet <clbrunet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 18:55:20 by clbrunet          #+#    #+#             */
/*   Updated: 2021/05/04 15:45:02 by clbrunet         ###   ########.fr       */
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

static char		initialize_threads_sems(t_globals *globs, t_thread *threads)
{
	sem_unlink(FORKS_SEM_NAME);
	globs->forks_sem = sem_open(FORKS_SEM_NAME, O_CREAT | O_EXCL, S_IRWXU,
			globs->nb_philosopher);
	if (globs->forks_sem == SEM_FAILED)
		return (1);
	sem_unlink(NB_DONE_SEM_NAME);
	globs->nb_done_sem = sem_open(NB_DONE_SEM_NAME, O_CREAT | O_EXCL, S_IRWXU,
			1);
	if (globs->forks_sem == SEM_FAILED)
	{
		sem_close(globs->forks_sem);
		sem_unlink(FORKS_SEM_NAME);
		return (1);
	}
	return (initialize_threads_death_sems(globs, threads));
}

static char		initialize_threads(t_globals *globs, t_thread *threads)
{
	unsigned int	i;

	if (initialize_threads_sems(globs, threads) != 0)
		return (1);
	i = 0;
	while (i < globs->nb_philosopher)
	{
		threads[i].arg.globs = globs;
		threads[i].arg.nb = i + 1;
		threads[i].arg.death_time = globs->time_to_die;
		i++;
	}
	return (0);
}

t_thread		*initialize_simulation(t_globals *globs)
{
	t_thread		*threads;

	threads = malloc((globs->nb_philosopher) * sizeof(t_thread));
	if (threads == NULL)
		return (NULL);
	if (initialize_threads(globs, threads) != 0)
	{
		free(threads);
		return (NULL);
	}
	return (threads);
}

void			destroy_simulation(t_globals *globs, t_thread *threads)
{
	unsigned int	i;
	char			*i_str;
	char			*sem_name;

	sem_close(globs->forks_sem);
	sem_unlink(FORKS_SEM_NAME);
	sem_close(globs->nb_done_sem);
	sem_unlink(NB_DONE_SEM_NAME);
	i = 0;
	while (i < globs->nb_philosopher)
	{
		sem_close(threads[i].arg.death_sem);
		i_str = ft_itoa(i + 1);
		if (i_str == NULL)
			return ;
		sem_name = ft_strjoin(DEATH_SEM_NAME_START, i_str);
		free(i_str);
		if (sem_name == NULL)
			return ;
		sem_unlink(sem_name);
		free(sem_name);
		i++;
	}
	free(threads);
}
