/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clbrunet <clbrunet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 09:43:02 by clbrunet          #+#    #+#             */
/*   Updated: 2021/05/06 18:44:44 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

static char		simulation_check_end(t_globals *globs, t_thread *threads)
{
	unsigned int	i;

	i = 0;
	while (i < globs->nb_philosopher)
	{
		if (globs->should_stop
				&& *threads[i].arg.nb_done == globs->nb_philosopher)
		{
			globs->is_finished = 1;
			return (1);
		}
		pthread_mutex_lock(&threads[i].arg.death_mutex);
		if (threads[i].arg.death_time <= get_time_ms() - globs->epoch)
		{
			globs->is_finished = 1;
			pthread_mutex_unlock(&threads[i].arg.death_mutex);
			printf("%07lu %i %s\n", get_time_ms() - globs->epoch, i + 1,
					"died");
			return (1);
		}
		pthread_mutex_unlock(&threads[i].arg.death_mutex);
		i++;
	}
	return (0);
}

static void		simulation(t_globals *globs, t_thread *threads)
{
	unsigned int	i;

	globs->epoch = get_time_ms();
	if (globs->epoch == (unsigned long)-1)
		return ;
	i = 0;
	while (i < globs->nb_philosopher)
	{
		if (pthread_create(&threads[i].id, NULL, &philo_routine,
					&threads[i].arg) != 0)
			return ;
		usleep(100);
		i++;
	}
	while (simulation_check_end(globs, threads) == 0)
		;
	i = 0;
	while (i < globs->nb_philosopher)
	{
		pthread_join(threads[i].id, NULL);
		i++;
	}
	return ;
}

int				main(int argc, char *argv[])
{
	t_globals		globs;
	t_thread		*threads;
	unsigned int	nb_done;
	pthread_mutex_t	nb_done_mutex;

	if (parse_args(argc, argv, &globs) != 0)
	{
		printf("philo_one usage :\n\tpath/philo_one number_of_philosopher "
				"time_to_die time_to_eat time_to_sleep "
				"[number_of_time_each_philosophers_must_eat]\n");
		return (1);
	}
	nb_done = 0;
	threads = initialize_simulation(&globs, &nb_done, &nb_done_mutex);
	if (threads == NULL)
		return (1);
	simulation(&globs, threads);
	destroy_simulation(&globs, threads);
	return (0);
}
