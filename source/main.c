/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 18:10:47 by mika              #+#    #+#             */
/*   Updated: 2025/08/14 16:12:48 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	*philo_routine(void *v)
{
	t_philo	*philo;

	philo = (t_philo *)v;
	waituntil(philo->conf->start_time);
	pthread_mutex_lock(&(philo->eating_lock));
	philo->last_meal = get_ms();
	pthread_mutex_unlock(&(philo->eating_lock));
	dynamicstagger(philo);
	while (!any_deaths(philo->conf))
	{
		if (amialive(philo) && !any_deaths(philo->conf))
			safeprint(philo, "is thinking");
		if (amialive(philo))
			lock_forks(philo);
		if (amialive(philo) && !any_deaths(philo->conf))
			safeprint(philo, "is eating");
		set_eating(philo, true);
		precise_sleep(philo->conf->eat_time);
		set_eating(philo, false);
		unlock_forks(philo);
		if (amialive(philo) && !any_deaths(philo->conf))
			safeprint(philo, "is sleeping");
		precise_sleep(philo->conf->sleep_time);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_config	conf;
	t_philo		*philos;
	pthread_t	monitor;

	philos = NULL;
	if ((argc != 5 && argc != 6) || !parse_conf(argc, argv, &conf))
		return (printf(ARGS_ISSUE), 1);
	conf.any_death = false;
	pthread_mutex_init(&conf.print_lock, NULL);
	pthread_mutex_init(&conf.death_lock, NULL);
	if (!init_mutex_pool(&conf))
		return (1);
	conf.start_time = get_ms() + 1000;
	philos = init_philo_structs(&conf);
	conf.philos = philos;
	pthread_create(&monitor, NULL, death_monitor, &conf);
	join_philo_threads(&conf);
	pthread_join(monitor, NULL);
	if (philos)
		free(philos);
	destroy_mutexes(&conf);
	free(conf.forks_pool);
}
