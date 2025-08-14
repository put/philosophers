/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:56:01 by mschippe          #+#    #+#             */
/*   Updated: 2025/08/14 16:24:19 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

long long	get_ms(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000);
}

void	precise_sleep(long long duration)
{
	long long	start;

	start = get_ms();
	while ((get_ms() - start) < duration)
		usleep(100);
}

void	waituntil(long long time)
{
	long long	newtime;

	newtime = get_ms();
	while (newtime < time)
	{
		precise_sleep(1);
		newtime = get_ms();
	}
}

void	dynamicstagger(t_philo *philo)
{
	if (philo->conf->num_phi <= 20)
		usleep(1000 * philo->id);
	else if (philo->conf->num_phi <= 100)
		usleep(500 * (philo->id % 10));
	else
		usleep(1500 * (philo->id % 2));
}
