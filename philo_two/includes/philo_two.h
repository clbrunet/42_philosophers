/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clbrunet <clbrunet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 09:44:26 by clbrunet          #+#    #+#             */
/*   Updated: 2021/05/06 13:52:11 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_TWO_H
# define PHILO_TWO_H

# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>
# include <limits.h>
# include <stdlib.h>
# include <pthread.h>
# include <semaphore.h>
# include <fcntl.h>

# define FORKS_SEM_NAME "/forks_sem"
# define NB_DONE_SEM_NAME "/nb_done_sem"
# define DEATH_SEM_NAME_START "/death_sem_"

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
	unsigned int	nb_done;
	sem_t			*nb_done_sem;
	sem_t			*forks_sem;
}				t_globals;

typedef struct	s_thread_arg
{
	t_globals		*globs;
	unsigned int	nb;
	sem_t			*death_sem;
	unsigned long	death_time;
}				t_thread_arg;

typedef struct	s_thread
{
	pthread_t		id;
	t_thread_arg	arg;
}				t_thread;

char			*ft_itoa(int n);
char			*ft_strjoin(char const *s1, char const *s2);

char			parse_args(int argc, char *argv[], t_globals *globs);
unsigned long	get_time_ms(void);
char			initialize_threads_death_sems(t_globals *globs,
		t_thread *threads);
t_thread		*initialize_simulation(t_globals *globs);
void			destroy_simulation(t_globals *globs, t_thread *threads);
void			*philo_routine(void *void_arg);

#endif
