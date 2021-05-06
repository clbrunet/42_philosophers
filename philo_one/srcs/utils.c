/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clbrunet <clbrunet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 18:55:20 by clbrunet          #+#    #+#             */
/*   Updated: 2021/05/06 14:39:06 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

unsigned long	get_time_ms(void)
{
	struct timeval	timeval;

	if (gettimeofday(&timeval, NULL) != 0)
		return (-1);
	return (timeval.tv_sec * 1000 + timeval.tv_usec / 1000);
}

static char		initialize_threads_mutexes(t_globals const *globs,
		t_thread *threads, pthread_mutex_t *nb_done_mutex)
{
	unsigned int	i;
	int				ret;

	if (pthread_mutex_init(nb_done_mutex, NULL) != 0)
		return (1);
	i = 0;
	while (i < globs->nb_philosopher)
	{
		ret = pthread_mutex_init(&threads[i].arg.death_mutex, NULL);
		if (ret || pthread_mutex_init(&threads[i].arg.left_fork_mutex, NULL))
		{
			pthread_mutex_destroy(nb_done_mutex);
			if (ret == 0)
				pthread_mutex_destroy(&threads[i].arg.death_mutex);
			while (i > 0)
			{
				i--;
				pthread_mutex_destroy(&threads[i].arg.left_fork_mutex);
				pthread_mutex_destroy(&threads[i].arg.death_mutex);
			}
			return (1);
		}
		i++;
	}
	return (0);
}

static char		initialize_threads(t_globals const *globs, t_thread *threads,
		unsigned int *nb_done, pthread_mutex_t *nb_done_mutex)
{
	unsigned int	i;

	if (initialize_threads_mutexes(globs, threads, nb_done_mutex) != 0)
		return (1);
	i = 0;
	while (i < globs->nb_philosopher)
	{
		threads[i].arg.globs = globs;
		threads[i].arg.nb = i + 1;
		if (i == 0)
			threads[i].arg.right_fork_mutex = &threads[globs->nb_philosopher
				- 1].arg.left_fork_mutex;
		else
			threads[i].arg.right_fork_mutex = &threads[i
				- 1].arg.left_fork_mutex;
		threads[i].arg.death_time = globs->time_to_die;
		threads[i].arg.nb_done_mutex = nb_done_mutex;
		threads[i].arg.nb_done = nb_done;
		i++;
	}
	return (0);
}

t_thread		*initialize_simulation(t_globals const *globs,
		unsigned int *nb_done, pthread_mutex_t *nb_done_mutex)
{
	t_thread		*threads;

	threads = malloc((globs->nb_philosopher) * sizeof(t_thread));
	if (threads == NULL)
		return (NULL);
	if (initialize_threads(globs, threads, nb_done, nb_done_mutex) != 0)
	{
		free(threads);
		return (NULL);
	}
	return (threads);
}

void			destroy_simulation(t_globals *globs, t_thread *threads)
{
	unsigned int	i;

	i = 0;
	pthread_mutex_destroy(threads[i].arg.nb_done_mutex);
	while (i < globs->nb_philosopher)
	{
		pthread_mutex_destroy(&threads[i].arg.left_fork_mutex);
		pthread_mutex_destroy(&threads[i].arg.death_mutex);
		i++;
	}
	free(threads);
}
