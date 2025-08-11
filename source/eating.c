/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 20:00:31 by mschippe          #+#    #+#             */
/*   Updated: 2025/08/11 20:00:46 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	lock_forks(t_philo *philo)
{
	if (philo->conf->num_phi == 1)
	{
		usleep((philo->conf->die_time) * 1000);
		return ;
	}
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

void	unlock_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

void	set_eating(t_philo *philo, bool value)
{
	pthread_mutex_lock(&(philo->eating_lock));
	philo->is_eating = value;
	philo->last_meal = get_ms();
	if (!value)
		philo->times_ate++;
	pthread_mutex_unlock(&(philo->eating_lock));
}
