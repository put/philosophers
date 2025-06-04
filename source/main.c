/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 18:10:47 by mika              #+#    #+#             */
/*   Updated: 2025/06/04 19:32:41 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

bool	parse_conf(int argc, char **argv, t_config *conf)
{
	return (ft_atoi(argv[1], &conf->num_phi)
			&& ft_atoi(argv[2], &conf->die_time)
			&& ft_atoi(argv[3], &conf->eat_time)
			&& ft_atoi(argv[4], &conf->sleep_time)
			&& (argc == 5 || ft_atoi(argv[5], &conf->num_eat))
			&& conf->die_time > 0
			&& conf->eat_time > 0
			&& conf->sleep_time > 0
			&& conf->num_phi > 0
			&& (argc == 5 || conf->num_eat > 0));
}

void	*print_thread_id(void * arg)
{
	(void)arg;
	printf("My TID: %lu\n", (unsigned long)pthread_self());
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_config	conf;
	pthread_t	one;
	pthread_t	two;

	if ((argc != 5 && argc != 6) || !parse_conf(argc, argv, &conf))
		return (printf(ARGS_ISSUE), 1);
	print_thread_id(NULL);
	pthread_create(&one, NULL, print_thread_id, NULL);
	pthread_create(&two, NULL, print_thread_id, NULL);
	pthread_join(one, NULL);
	pthread_join(two, NULL);
}