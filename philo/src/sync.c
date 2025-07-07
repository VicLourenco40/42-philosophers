/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vde-albu <vde-albu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:57:17 by vde-albu          #+#    #+#             */
/*   Updated: 2025/07/07 11:19:40 by vde-albu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <unistd.h>

void	manager_sync(t_params *const params, t_philo *const philos)
{
	int	philos_ready;
	int	i;

	while (1)
	{
		pthread_mutex_lock(&params->mutex);
		philos_ready = params->philos_ready == params->num_philos;
		pthread_mutex_unlock(&params->mutex);
		if (philos_ready)
			break ;
		usleep(1);
	}
	i = -1;
	params->start = get_timestamp();
	while (++i < params->num_philos)
		philos[i].last_meal = params->start;
	pthread_mutex_lock(&params->mutex);
	params->manager_ready = 1;
	pthread_mutex_unlock(&params->mutex);
}

void	philo_sync(t_params *const params)
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
		usleep(1);
	}
}

int	check_stop(t_params *const params)
{
	int	stop;

	pthread_mutex_lock(&params->mutex);
	stop = params->stop;
	pthread_mutex_unlock(&params->mutex);
	return (stop);
}
