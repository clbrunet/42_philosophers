/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clbrunet <clbrunet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 09:43:02 by clbrunet          #+#    #+#             */
/*   Updated: 2021/05/04 14:58:43 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

static char		simulation_check_end(t_globals *globs, t_thread *threads)
{
	unsigned int	i;

	i = 0;
	while (i < globs->nb_philosopher)
	{
		if (globs->should_stop
				&& globs->nb_done == globs->nb_philosopher)
		{
			globs->is_finished = 1;
			return (1);
		}
		sem_wait(threads[i].arg.death_sem);
		if (threads[i].arg.death_time <= get_time_ms() - globs->epoch)
		{
			globs->is_finished = 1;
			sem_post(threads[i].arg.death_sem);
			printf("%07lu %i %s\n", get_time_ms() - globs->epoch, i + 1,
					"died");
			return (1);
		}
		sem_post(threads[i].arg.death_sem);
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

	if (parse_args(argc, argv, &globs) != 0)
	{
		printf("philo_two usage :\n\tpath/philo_two number_of_philosopher "
				"time_to_die time_to_eat time_to_sleep "
				"[number_of_time_each_philosophers_must_eat]\n");
		return (1);
	}
	threads = initialize_simulation(&globs);
	if (threads == NULL)
		return (1);
	simulation(&globs, threads);
	destroy_simulation(&globs, threads);
	return (0);
}
