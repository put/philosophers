/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 20:01:52 by mschippe          #+#    #+#             */
/*   Updated: 2025/08/11 20:02:05 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

t_philo	*init_philo_structs(t_config *conf)
{
	int		i;
	t_philo	*philos;

	i = 0;
	philos = malloc(sizeof(t_philo) * conf->num_phi);
	if (!philos)
		return (NULL);
	while (i < conf->num_phi)
	{
		philos[i].id = i + 1;
		philos[i].last_meal = conf->start_time;
		philos[i].conf = conf;
		philos[i].l_fork = &(conf->forks_pool[i]);
		philos[i].r_fork = &(conf->forks_pool[(i + 1) % conf->num_phi]);
		philos[i].is_eating = false;
		philos[i].times_ate = 0;
		pthread_mutex_init(&(philos[i].eating_lock), NULL);
		if (i % 2 == 0)
			usleep(1250);
		pthread_create(&(philos[i].thread), NULL, philo_routine, &(philos[i]));
		i++;
	}
	return (philos);
}

void	join_philo_threads(t_config *conf)
{
	int	i;

	i = 0;
	while (i < conf->num_phi)
		pthread_join(conf->philos[i++].thread, NULL);
}
