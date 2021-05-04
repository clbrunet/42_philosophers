/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clbrunet <clbrunet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 09:44:26 by clbrunet          #+#    #+#             */
/*   Updated: 2021/05/04 08:25:33 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>
# include <limits.h>
# include <stdlib.h>
# include <pthread.h>

typedef struct	s_globals
{
	unsigned int	nb_philosopher;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	char			should_stop;
	unsigned int	nb_time_philosophers_must_eat;
	unsigned long	epoch;
	char			is_finished;
}				t_globals;

typedef struct	s_thread_arg
{
	t_globals const	*globs;
	unsigned int	nb;
	pthread_mutex_t	left_fork_mutex;
	pthread_mutex_t	*right_fork_mutex;
	pthread_mutex_t	death_mutex;
	unsigned long	death_time;
	pthread_mutex_t	*nb_done_mutex;
	unsigned int	*nb_done;
}				t_thread_arg;

typedef struct	s_thread
{
	pthread_t		id;
	t_thread_arg	arg;
}				t_thread;

char			parse_args(int argc, char *argv[], t_globals *globs);
unsigned long	get_time_ms(void);
t_thread		*initialize_simulation(t_globals const *globs,
		unsigned int *nb_done, pthread_mutex_t *nb_done_mutex);
void			destroy_simulation(t_globals *globs, t_thread *threads);
void			*philo_routine(void *void_arg);

#endif
