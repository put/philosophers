/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:59:19 by mschippe          #+#    #+#             */
/*   Updated: 2025/08/11 20:32:41 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

bool	init_mutex_pool(t_config *conf)
{
	int	i;

	i = 0;
	conf->forks_pool = malloc(sizeof(pthread_mutex_t) * conf->num_phi);
	if (!conf->forks_pool)
		return (false);
	while (i < conf->num_phi)
	{
		if (pthread_mutex_init(&conf->forks_pool[i], NULL))
			break ;
		i++;
	}
	if (i < conf->num_phi)
	{
		while (i >= 0)
			pthread_mutex_destroy(&conf->forks_pool[i--]);
		free(conf->forks_pool);
		conf->forks_pool = NULL;
		return (false);
	}
	return (true);
}

void	destroy_mutexes(t_config *conf)
{
	int	i;

	i = 0;
	while (i < conf->num_phi)
		pthread_mutex_destroy(&(conf->forks_pool[i++]));
}
