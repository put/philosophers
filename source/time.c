/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:56:01 by mschippe          #+#    #+#             */
/*   Updated: 2025/08/11 20:32:01 by mschippe         ###   ########.fr       */
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
