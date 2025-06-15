/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 18:10:47 by mika              #+#    #+#             */
/*   Updated: 2025/06/15 17:27:09 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"


bool	parse_conf(int argc, char **argv, t_config *conf)
{
	return (ft_atoi(argv[1], &conf->num_phi)
			&& ft_atoi(argv[2], &conf->die_time)
			&& ft_atoi(argv[3], &conf->eat_time)
			&& ft_atoi(argv[4], &conf->sleep_time)
			&& (argc == 5 || ft_atoi(argv[5], &conf->num_eat))
			&& conf->die_time > 0
			&& conf->eat_time > 0
			&& conf->sleep_time > 0
			&& conf->num_phi > 0
			&& (argc == 5 || conf->num_eat > 0));
}

void	*print_thread_id(void * arg)
{
	(void)arg;
	printf("My TID: %lu\n", (unsigned long)pthread_self());
	return (NULL);
}

long long	get_ms(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_usec / 1000);
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

void	destroy_mutexes(t_config *conf)
{
	int	i;

	i = 0;
	while (i < conf->num_phi)
		pthread_mutex_destroy(&(conf->forks_pool[i++]));
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
		philos[i].id = i;
		philos[i].last_meal = conf->start_time;
		philos[i].conf = conf;
		philos[i].l_fork = conf->forks_pool[i];
		philos[i].r_fork = conf->forks_pool[(i + 1) % conf->num_phi];
		pthread_create(&(philos[i].thread), NULL, print_thread_id, NULL);
		i++;
	}
	i = 0;
	while (i < conf->num_phi)
		pthread_join(philos[i++].thread, NULL);
	return (philos);
}

int	main(int argc, char **argv)
{
	t_config	conf;
	t_philo		*philos;

	philos = NULL;
	if ((argc != 5 && argc != 6) || !parse_conf(argc, argv, &conf))
		return (printf(ARGS_ISSUE), 1);
	conf.start_time = get_ms();
	if (!init_mutex_pool(&conf))
		return (1); //TODO: Do something to destroy everything :)
	philos = init_philo_structs(&conf);
	if (philos)
		free(philos);
	destroy_mutexes(&conf);
	free(conf.forks_pool);
}
