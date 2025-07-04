/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   funcs3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 17:39:31 by mschippe          #+#    #+#             */
/*   Updated: 2025/07/04 18:32:11 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	destroy_mutexes(t_config *conf)
{
	int	i;

	i = 0;
	while (i < conf->num_phi)
	{
		pthread_mutex_destroy(&(conf->forks_pool[i++]));
		pthread_mutex_destroy(&(conf->philos[i].eating_lock));
	}
	pthread_mutex_destroy(&conf->print_lock);
	pthread_mutex_destroy(&conf->death_lock);
	pthread_mutex_destroy(&conf->total_ate_lock);
}

bool	amialive(t_philo *philo)
{
	bool		is_eating;
	long long	last_meal;

	pthread_mutex_lock(&(philo->eating_lock));
	is_eating = philo->is_eating;
	last_meal = philo->last_meal;
	pthread_mutex_unlock(&(philo->eating_lock));
	return (!((last_meal + philo->conf->die_time < get_ms()) && !is_eating));
}

bool	any_deaths(t_config *conf)
{
	bool	has_deaths;

	pthread_mutex_lock(&(conf->death_lock));
	has_deaths = conf->any_death;
	pthread_mutex_unlock(&(conf->death_lock));
	return (has_deaths);
}

bool	eat_limit(t_config *conf)
{
	bool	reached;

	if (conf->n_eat <= 0)
		return (false);
	reached = false;
	pthread_mutex_lock(&conf->total_ate_lock);
	reached = (conf->n_eat * conf->num_phi) <= conf->total_ate;
	pthread_mutex_unlock(&conf->total_ate_lock);
	return (reached);
}

void	lock_forks(t_philo *philo)
{
	if (philo->id % 2)
	{
		pthread_mutex_lock(philo->r_fork);
		if (amialive(philo) && !any_deaths(philo->conf))
		{
			safeprint(philo, "has taken a fork");
			pthread_mutex_lock(philo->l_fork);
			if (amialive(philo) && !any_deaths(philo->conf))
				safeprint(philo, "has taken a fork");
		}
		return ;
	}
	pthread_mutex_lock(philo->l_fork);
	if (amialive(philo) && !any_deaths(philo->conf))
	{
		safeprint(philo, "has taken a fork");
		pthread_mutex_lock(philo->r_fork);
		if (amialive(philo) && !any_deaths(philo->conf))
			safeprint(philo, "has taken a fork");
	}
}
