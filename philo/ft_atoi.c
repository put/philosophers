/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 18:17:47 by mika              #+#    #+#             */
/*   Updated: 2025/06/04 18:42:58 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdbool.h>

bool	ft_atoi(const char *nptr, int *target)
{
	long	sig;
	long	res;
	bool	success;

	res = 0;
	sig = 1;
	success = false;
	while ((*nptr > 8 && *nptr < 14) || *nptr == ' ')
		sig = (*nptr++ || res == 0);
	if (*nptr == 43 || *nptr == 45)
		sig = (*nptr++ - 44) * -1;
	while (*nptr >= '0' && *nptr <= '9')
	{
		res = res * 10 + (*nptr++ - 48);
		if (res > INT_MAX || res < INT_MIN)
			return (false);
		success = true;
	}
	if (*nptr || !success)
		return (false);
	*target = (int)(sig * res);
	return (true);
}
