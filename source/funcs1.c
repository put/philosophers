/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   funcs1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 17:34:46 by mschippe          #+#    #+#             */
/*   Updated: 2025/07/04 17:49:54 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	unlock_forks(t_philo *philo)
{
	if (philo->id % 2)
	{
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		return ;
	}
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}

void	set_eating(t_philo *philo, bool value)
{
	pthread_mutex_lock(&(philo->eating_lock));
	philo->is_eating = value;
	if (value)
		philo->last_meal = get_ms();
	if (!value)
	{
		philo->times_ate++;
		if (philo->conf->n_eat > 0 && philo->times_ate <= philo->conf->n_eat)
		{
			pthread_mutex_lock(&philo->conf->total_ate_lock);
			philo->conf->total_ate++;
			pthread_mutex_unlock(&philo->conf->total_ate_lock);
		}
	}
	pthread_mutex_unlock(&(philo->eating_lock));
}

void	*philo_routine(void *v)
{
	t_philo	*philo;

	philo = (t_philo *)v;
	waituntil(philo->conf->start_time + (philo->id % 2));
	while (!any_deaths(philo->conf) && !eat_limit(philo->conf))
	{
		if (amialive(philo))
			lock_forks(philo);
		if (amialive(philo) && !any_deaths(philo->conf))
			safeprint(philo, "is eating");
		set_eating(philo, true);
		usleep(philo->conf->eat_time * 1000);
		if (!amialive(philo) || any_deaths(philo->conf))
			break ;
		set_eating(philo, false);
		unlock_forks(philo);
		if (amialive(philo) && !any_deaths(philo->conf))
			safeprint(philo, "is sleeping");
		usleep(philo->conf->sleep_time * 1000);
		if (amialive(philo) && !any_deaths(philo->conf))
			safeprint(philo, "is thinking");
	}
	return (NULL);
}

void	*death_monitor(void *v)
{
	int			i;
	t_config	*conf;

	conf = (t_config *)v;
	i = 0;
	while (!any_deaths(conf) && !eat_limit(conf))
	{
		while (i++ < conf->num_phi)
		{
			if (!amialive(&(conf->philos[i - 1])))
			{
				safeprint(&(conf->philos[i - 1]), "died");
				pthread_mutex_lock(&(conf->death_lock));
				conf->any_death = true;
				pthread_mutex_unlock(&(conf->death_lock));
				i = -1;
				break ;
			}
		}
		if (i == -1)
			break ;
		i = 0;
		usleep(100);
	}
	return (NULL);
}

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
		pthread_create(&(philos[i].thread), NULL, philo_routine, &(philos[i]));
		i++;
	}
	return (philos);
}
