/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:58:41 by mschippe          #+#    #+#             */
/*   Updated: 2025/08/11 20:00:56 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	safeprint(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&(philo->conf->print_lock));
	printf("%lld %d %s\n", get_ms() - philo->conf->start_time, philo->id, msg);
	pthread_mutex_unlock(&(philo->conf->print_lock));
}
