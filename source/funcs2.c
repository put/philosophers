/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   funcs2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 17:36:31 by mschippe          #+#    #+#             */
/*   Updated: 2025/07/04 17:37:12 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

long long	get_ms(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000);
}

void	waituntil(long long time)
{
	long long	newtime;

	newtime = get_ms();
	while (newtime < time)
	{
		usleep(1000);
		newtime = get_ms();
	}
}

void	safeprint(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&(philo->conf->print_lock));
	printf("%lld %d %s\n", get_ms() - philo->conf->start_time, philo->id, msg);
	pthread_mutex_unlock(&(philo->conf->print_lock));
}

bool	parse_conf(int argc, char **argv, t_config *conf)
{
	return (ft_atoi(argv[1], &conf->num_phi)
		&& ft_atoi(argv[2], &conf->die_time)
		&& ft_atoi(argv[3], &conf->eat_time)
		&& ft_atoi(argv[4], &conf->sleep_time)
		&& (argc == 5 || ft_atoi(argv[5], &conf->n_eat))
		&& conf->die_time > 0
		&& conf->eat_time > 0
		&& conf->sleep_time > 0
		&& conf->num_phi > 0
		&& (argc == 5 || conf->n_eat > 0));
}

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
