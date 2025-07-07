/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vde-albu <vde-albu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 17:34:52 by vde-albu          #+#    #+#             */
/*   Updated: 2025/07/07 10:39:11 by vde-albu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <stdio.h>
#include <unistd.h>

static void	give_forks(t_params *const params, t_philo *const philos,
	const int first)
{
	int	i;
	int	t;

	i = -1;
	while (++i < params->num_philos / 2)
	{
		t = (first + i * 2) % params->num_philos;
		pthread_mutex_lock(&philos[t].forks[0]->mutex);
		pthread_mutex_lock(&philos[t].forks[1]->mutex);
		philos[t].forks[0]->user = t;
		philos[t].forks[1]->user = t;
		pthread_mutex_unlock(&philos[t].forks[1]->mutex);
		pthread_mutex_unlock(&philos[t].forks[0]->mutex);
	}
}

static int	check_eating(t_params *const params, t_philo *const philos,
	const int first)
{
	int	i;
	int	t;
	int	eating;

	if (params->num_philos == 1)
		return (1);
	i = -1;
	while (++i < params->num_philos / 2)
	{
		t = (first + i * 2) % params->num_philos;
		pthread_mutex_lock(&philos[t].forks[0]->mutex);
		pthread_mutex_lock(&philos[t].forks[1]->mutex);
		eating = philos[t].forks[0]->user == t && philos[t].forks[1]->user == t;
		pthread_mutex_unlock(&philos[t].forks[1]->mutex);
		pthread_mutex_unlock(&philos[t].forks[0]->mutex);
		if (eating)
			return (1);
	}
	return (0);
}

static int	check_dead(t_params *const params, t_philo *const philos)
{
	int	i;
	int	dead;

	i = -1;
	while (++i < params->num_philos)
	{
		pthread_mutex_lock(&philos[i].mutex);
		dead = get_timestamp() - philos[i].last_meal >= params->time_to_die;
		if (dead)
		{
			pthread_mutex_lock(&params->print_mutex);
			printf("%lu\t%d died\n", get_timestamp() - params->start, i + 1);
			pthread_mutex_unlock(&params->print_mutex);
			pthread_mutex_lock(&params->mutex);
			params->stop = 1;
			pthread_mutex_unlock(&params->mutex);
		}
		pthread_mutex_unlock(&philos[i].mutex);
		if (dead)
			return (1);
	}
	return (0);
}

static int	check_min_meals(t_params *const params, t_philo *const philos)
{
	int	i;
	int	under;

	if (params->min_meals <= 0)
		return (0);
	i = -1;
	while (++i < params->num_philos)
	{
		pthread_mutex_lock(&philos[i].mutex);
		under = philos[i].num_meals < params->min_meals;
		pthread_mutex_unlock(&philos[i].mutex);
		if (under)
			return (0);
	}
	pthread_mutex_lock(&params->mutex);
	params->stop = 1;
	pthread_mutex_unlock(&params->mutex);
	return (1);
}

void	manager(t_params *const params, t_philo *const philos)
{
	int	first;

	manager_sync(params, philos);
	first = 0;
	while (1)
	{
		give_forks(params, philos, first);
		while (check_eating(params, philos, first))
		{
			if (check_dead(params, philos))
				return ;
			usleep(1);
		}
		if (check_min_meals(params, philos))
			return ;
		first = (first + 1) % params->num_philos;
	}
}
