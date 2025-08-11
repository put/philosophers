/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 20:04:25 by mschippe          #+#    #+#             */
/*   Updated: 2025/08/11 20:35:48 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

bool	any_deaths(t_config *conf)
{
	bool	has_deaths;

	pthread_mutex_lock(&(conf->death_lock));
	has_deaths = conf->any_death;
	pthread_mutex_unlock(&(conf->death_lock));
	return (has_deaths);
}

bool	amialive(t_philo *philo)
{
	bool		is_eating;
	long long	last_meal;

	pthread_mutex_lock(&(philo->eating_lock));
	is_eating = philo->is_eating;
	last_meal = philo->last_meal;
	pthread_mutex_unlock(&(philo->eating_lock));
	return (!((last_meal + philo->conf->die_time < get_ms())
			&& !is_eating) && !any_deaths(philo->conf));
}

bool	death_each_philo(t_config *conf, bool *all_ate, int *i)
{
	while (*i < conf->num_phi)
	{
		if (!amialive(&(conf->philos[*i])))
		{
			safeprint(&(conf->philos[*i]), "died");
			pthread_mutex_lock(&(conf->death_lock));
			conf->any_death = true;
			pthread_mutex_unlock(&(conf->death_lock));
			return (false);
		}
		if (conf->philos[*i].times_ate < conf->num_eat)
			*all_ate = false;
		(*i)++;
	}
	return (true);
}

void	*death_monitor(void *v)
{
	int			i;
	t_config	*conf;
	bool		all_ate;

	conf = (t_config *)v;
	i = 0;
	while (!any_deaths(conf))
	{
		all_ate = true;
		if (!death_each_philo(conf, &all_ate, &i))
			return (NULL);
		if (all_ate)
		{
			pthread_mutex_lock(&conf->death_lock);
			conf->any_death = true;
			pthread_mutex_unlock(&conf->death_lock);
			return (NULL);
		}
		i = 0;
		usleep(100);
	}
	return (NULL);
}
