/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vde-albu <vde-albu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:57:17 by vde-albu          #+#    #+#             */
/*   Updated: 2025/07/03 17:30:07 by vde-albu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <unistd.h>

void	manager_sync(t_params *const params, t_philo *const philos)
{
	int		philos_ready;
	long	timestamp;
	int		i;

	while (1)
	{
		pthread_mutex_lock(&params->mutex);
		philos_ready = params->philos_ready == params->num_philos;
		pthread_mutex_unlock(&params->mutex);
		if (philos_ready)
			break ;
		usleep(1);
	}
	timestamp = get_timestamp();
	i = -1;
	while (++i < params->num_philos)
		philos[i].last_meal = timestamp;
}

void	philosopher_sync(t_params *const params)
{
	pthread_mutex_lock(&params->mutex);
	params->philos_ready++;
	pthread_mutex_unlock(&params->mutex);
}
