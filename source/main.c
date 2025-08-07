/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 18:10:47 by mika              #+#    #+#             */
/*   Updated: 2025/08/07 16:11:01 by mschippe         ###   ########.fr       */
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
		usleep(100); // Sleep in very small increments
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
	return (!((last_meal + philo->conf->die_time < get_ms()) && !is_eating) && !any_deaths(philo->conf));
}


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
	if (!value && philo->times_ate < philo->conf->num_eat)
	{
		philo->times_ate++;
		philo->conf->total_ate++;
		if (philo->conf->total_ate
			>= philo->conf->num_eat * philo->conf->num_phi
			&& philo->conf->num_eat > 0)
		{
			pthread_mutex_lock(&(philo->conf->death_lock));
			philo->conf->any_death = true;
			pthread_mutex_unlock(&(philo->conf->death_lock));
		}
	}
	pthread_mutex_unlock(&(philo->eating_lock));
}

void	*philo_routine(void *v)
{
	t_philo	*philo;

	philo = (t_philo *)v;
	while (!any_deaths(philo->conf))
	{
		if (amialive(philo) && !any_deaths(philo->conf))
			safeprint(philo, "is thinking");
		if (amialive(philo))
			lock_forks(philo);
		if (amialive(philo) && !any_deaths(philo->conf))
			safeprint(philo, "is eating");
		set_eating(philo, true);
		precise_sleep(philo->conf->eat_time);
		set_eating(philo, false);
		unlock_forks(philo);
		if (amialive(philo) && !any_deaths(philo->conf))
			safeprint(philo, "is sleeping");
		precise_sleep(philo->conf->sleep_time);
	}
	return (NULL);
}

void	*death_monitor(void *v)
{
	int	i;
	t_config	*conf;

	conf = (t_config *)v;
	i = 0;
	while (!any_deaths(conf))
	{
		while (i < conf->num_phi)
		{
			if (!amialive(&(conf->philos[i])))
			{
				safeprint(&(conf->philos[i]), "died");
				pthread_mutex_lock(&(conf->death_lock));
				conf->any_death = true;
				pthread_mutex_unlock(&(conf->death_lock));
				return (NULL);
			}
			i++;
		}
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
		if (i % 2 == 0)
			usleep(750);
		pthread_create(&(philos[i].thread), NULL, philo_routine, &(philos[i]));
		i++;
	}
	return (philos);
}

void	join_philo_threads(t_config *conf)
{
	int	i;

	i = 0;
	while (i < conf->num_phi)
		pthread_join(conf->philos[i++].thread, NULL);
}

int	main(int argc, char **argv)
{
	t_config	conf;
	t_philo		*philos;
	pthread_t	monitor;

	philos = NULL;
	if ((argc != 5 && argc != 6) || !parse_conf(argc, argv, &conf))
		return (printf(ARGS_ISSUE), 1);
	conf.any_death = false;
	pthread_mutex_init(&conf.print_lock, NULL);
	pthread_mutex_init(&conf.death_lock, NULL);
	if (!init_mutex_pool(&conf))
		return (1); //TODO: Do something to destroy everything :)
	conf.start_time = get_ms();
	philos = init_philo_structs(&conf); //TODO: has malloc, need to check if fail
	conf.philos = philos;
	pthread_create(&monitor, NULL, death_monitor, &conf);
	join_philo_threads(&conf);
	pthread_join(monitor, NULL);
	if (philos)
		free(philos);
	destroy_mutexes(&conf);
	free(conf.forks_pool);
}
