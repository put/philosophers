/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 18:10:47 by mika              #+#    #+#             */
/*   Updated: 2025/07/04 18:27:49 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	join_philo_threads(t_config *conf)
{
	int	i;

	i = 0;
	while (i < conf->num_phi)
		pthread_join(conf->philos[i++].thread, NULL);
}

int	main(int argc, char **argv)
{
	t_config	conf;
	t_philo		*philos;
	pthread_t	monitor;

	philos = NULL;
	if ((argc != 5 && argc != 6) || !parse_conf(argc, argv, &conf))
		return (printf(ARGS_ISSUE), 1);
	conf.start_time = get_ms() + (conf.num_phi * 2);
	conf.any_death = false;
	conf.total_ate = 0;
	pthread_mutex_init(&conf.print_lock, NULL);
	pthread_mutex_init(&conf.death_lock, NULL);
	pthread_mutex_init(&conf.total_ate_lock, NULL);
	if (!init_mutex_pool(&conf))
		return (1);
	philos = init_philo_structs(&conf);
	conf.philos = philos;
	pthread_create(&monitor, NULL, death_monitor, &conf);
	join_philo_threads(&conf);
	pthread_join(monitor, NULL);
	destroy_mutexes(&conf);
	if (philos)
		free(philos);
	free(conf.forks_pool);
}
