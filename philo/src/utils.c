/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vde-albu <vde-albu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 10:04:19 by vde-albu          #+#    #+#             */
/*   Updated: 2025/06/25 10:09:40 by vde-albu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	if (size && nmemb > (SIZE_MAX / size))
		return (NULL);
	ptr = malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	memset(ptr, 0, nmemb * size);
	return (ptr);
}

int	ft_atoi(const char *nptr)
{
	int	sign;
	int	num;

	while ((*nptr >= 9 && *nptr <= 13) || *nptr == ' ')
		nptr++;
	sign = 1;
	if (*nptr == '-' || *nptr == '+')
		if (*nptr++ == '-')
			sign = -1;
	num = 0;
	while (*nptr >= '0' && *nptr <= '9')
		num = num * 10 + *nptr++ - '0';
	return (num * sign);
}
