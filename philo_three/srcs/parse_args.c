/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clbrunet <clbrunet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 09:43:00 by clbrunet          #+#    #+#             */
/*   Updated: 2021/05/05 15:14:22 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

static int		ft_isdigit(int c)
{
	return ('0' <= c && c <= '9');
}

static char		check_args(int argc, char *argv[])
{
	char	*argument;

	if (argc != 5 && argc != 6)
		return (1);
	argv++;
	while (*argv)
	{
		argument = *argv;
		while (ft_isdigit(*argument))
			argument++;
		if (*argument != '\0')
			return (1);
		argv++;
	}
	return (0);
}

static unsigned	atou(char *nptr)
{
	unsigned int	n;

	n = 0;
	while (ft_isdigit(*nptr))
	{
		if ((unsigned long long)n * 10 + *nptr - '0'
			>= (unsigned long long)UINT_MAX)
			return (0);
		n = n * 10 + *nptr - '0';
		nptr++;
	}
	return (n);
}

char			parse_args(int argc, char *argv[], t_globals *globs)
{
	if (check_args(argc, argv) != 0)
		return (1);
	globs->nb_philosopher = atou(argv[1]);
	globs->time_to_die = atou(argv[2]);
	globs->time_to_eat = atou(argv[3]);
	globs->time_to_sleep = atou(argv[4]);
	if (argc == 6)
	{
		globs->should_stop = 1;
		globs->nb_time_philosophers_must_eat = atou(argv[5]);
	}
	else
		globs->should_stop = 0;
	if (globs->nb_philosopher < 2 || globs->time_to_die == 0
		|| globs->time_to_eat == 0 || globs->time_to_sleep == 0
		|| (argc == 6 && globs->nb_time_philosophers_must_eat == 0))
		return (1);
	return (0);
}
