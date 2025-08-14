/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:57:28 by mschippe          #+#    #+#             */
/*   Updated: 2025/08/14 16:59:58 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
