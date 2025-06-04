/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 18:12:30 by mika              #+#    #+#             */
/*   Updated: 2025/06/04 19:14:24 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# define ARGS_ISSUE "Incorrect argument(s) found.\nUsage: \
./philo [\e[1;31mnumber_of_philosophers\e[0m] \
[\e[1;31mtime_to_die\e[0m] [\e[1;31mtime_to_eat\e[0m] \
[\e[1;31mtime_to_sleep\e[0m] [\e[1;32mnumber_of_times_each\
_philosopher_must_eat\e[0m]\n"
# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>
# include <pthread.h>

typedef struct s_config
{
	int	num_phi;
	int	die_time;
	int	eat_time;
	int	sleep_time;
	int	num_eat;
}	t_config;

bool	ft_atoi(const char *nptr, int *target);
#endif