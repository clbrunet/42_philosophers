/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clbrunet <clbrunet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 09:44:26 by clbrunet          #+#    #+#             */
/*   Updated: 2021/05/06 14:41:08 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_THREE_H
# define PHILO_THREE_H

# include <stdio.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <unistd.h>
# include <limits.h>
# include <stdlib.h>
# include <pthread.h>
# include <semaphore.h>
# include <fcntl.h>

# define FORKS_SEM_NAME "/forks_sem"
# define NB_DN_SEM_NAME "/nb_done_sem"
# define IS_FINISHED_SEM_NAME "/is_finished_sem"
# define OUTPUT_SEM_NAME "/output_sem_"
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
	sem_t			*is_finished_sem;
	sem_t			*nb_done_sem;
	sem_t			*forks_sem;
	sem_t			*output_sem;
}				t_globals;

typedef struct	s_process_arg
{
	t_globals		*globs;
	unsigned int	nb;
	sem_t			*death_sem;
	unsigned long	death_time;
	char			is_finished;
}				t_process_arg;

char			*ft_itoa(int n);
char			*ft_strjoin(char const *s1, char const *s2);

char			parse_args(int argc, char *argv[], t_globals *globs);
unsigned long	get_time_ms(void);
char			initialize_simulation(t_globals *globs,
		t_process_arg *process_arg);
void			destroy_simulation(t_globals *globs);

int				philo_routine(t_process_arg *arg);
int				init_death_sem(t_process_arg *arg);
void			close_death_sem(t_process_arg *arg);
int				philo_process(t_process_arg *arg);

#endif
