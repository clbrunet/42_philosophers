/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clbrunet <clbrunet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 09:43:02 by clbrunet          #+#    #+#             */
/*   Updated: 2021/05/05 20:07:24 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

static char		create_philo_process(pid_t *pids, unsigned int i,
		t_process_arg *process_arg)
{
	process_arg->nb = i + 1;
	pids[i] = fork();
	if (pids[i] == 0)
	{
		free(pids);
		exit(philo_process(process_arg));
	}
	else if (pids[i] == -1)
	{
		sem_post(process_arg->globs->is_finished_sem);
		return (1);
	}
	return (0);
}

static void		*check_nb_done_routine(void *void_arg)
{
	t_globals		*globs;
	unsigned int	i;

	globs = (t_globals *)void_arg;
	i = 0;
	while (i < globs->nb_philosopher)
	{
		sem_wait(globs->nb_done_sem);
		i++;
	}
	sem_post(globs->is_finished_sem);
	return (NULL);
}

static void		start_check_nb_done_thread(t_globals *globs)
{
	pthread_t	check_nb_done_th;

	if (pthread_create(&check_nb_done_th, NULL, check_nb_done_routine, globs)
			!= 0)
		sem_post(globs->is_finished_sem);
	pthread_join(check_nb_done_th, NULL);
	return ;
}

static void		simulation(t_globals *globs, t_process_arg *process_arg)
{
	unsigned int	i;
	pid_t			*pids;

	pids = malloc((globs->nb_philosopher) * sizeof(pid_t));
	if (pids == NULL)
		return ;
	globs->epoch = get_time_ms();
	if (globs->epoch == (unsigned long)-1)
		return ;
	i = 0;
	while (i < globs->nb_philosopher)
	{
		if (create_philo_process(pids, i, process_arg) != 0)
			break ;
		i++;
	}
	free(pids);
	if (i == globs->nb_philosopher && globs->should_stop)
		start_check_nb_done_thread(globs);
	while (i > 0)
	{
		waitpid(-1, NULL, 0);
		i--;
	}
	return ;
}

int				main(int argc, char *argv[])
{
	t_globals		globs;
	t_process_arg	process_arg;

	if (parse_args(argc, argv, &globs) != 0)
	{
		printf("philo_three usage :\n\tpath/philo_three number_of_philosopher "
				"time_to_die time_to_eat time_to_sleep "
				"[number_of_time_each_philosophers_must_eat]\n");
		return (1);
	}
	initialize_simulation(&globs, &process_arg);
	simulation(&globs, &process_arg);
	destroy_simulation(&globs);
	return (0);
}
