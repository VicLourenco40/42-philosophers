/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vde-albu <vde-albu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:57:17 by vde-albu          #+#    #+#             */
/*   Updated: 2025/07/02 16:00:29 by vde-albu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <unistd.h>

void	manager_sync(t_params *const params)
{
	int	philos_ready;

	while (1)
	{
		pthread_mutex_lock(&params->mutex);
		philos_ready = params->philos_ready == params->num_philos;
		pthread_mutex_unlock(&params->mutex);
		if (philos_ready)
		{
			pthread_mutex_lock(&params->mutex);
			params->manager_ready = 1;
			pthread_mutex_unlock(&params->mutex);
			return ;
		}
		usleep(100);
	}
}

void	philosopher_sync(t_params *const params)
{
	int	manager_ready;

	pthread_mutex_lock(&params->mutex);
	params->philos_ready++;
	pthread_mutex_unlock(&params->mutex);
	while (1)
	{
		pthread_mutex_lock(&params->mutex);
		manager_ready = params->manager_ready;
		pthread_mutex_unlock(&params->mutex);
		if (manager_ready)
			return ;
		usleep(100);
	}
}
