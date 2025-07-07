/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vde-albu <vde-albu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:40:11 by vde-albu          #+#    #+#             */
/*   Updated: 2025/07/07 11:19:32 by vde-albu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <unistd.h>
#include <stdio.h>

static int	check_can_eat(t_philo *const philo, t_params *const params)
{
	int	can_eat;

	if (params->num_philos == 1)
		return (0);
	pthread_mutex_lock(&philo->forks[0]->mutex);
	pthread_mutex_lock(&philo->forks[1]->mutex);
	can_eat = philo->forks[0]->user == philo->index && \
		philo->forks[1]->user == philo->index;
	pthread_mutex_unlock(&philo->forks[1]->mutex);
	pthread_mutex_unlock(&philo->forks[0]->mutex);
	return (can_eat);
}

static int	philo_think(t_philo *const philo, t_params *const params)
{
	pthread_mutex_lock(&params->mutex);
	if (params->stop)
		return (pthread_mutex_unlock(&params->mutex), 1);
	pthread_mutex_lock(&philo->params->print_mutex);
	printf("%lu\t%d is thinking\n", get_timestamp() - philo->params->start,
		philo->index + 1);
	pthread_mutex_unlock(&philo->params->print_mutex);
	pthread_mutex_unlock(&params->mutex);
	return (0);
}

static int	philo_eat(t_philo *const philo, t_params *const params)
{
	pthread_mutex_lock(&philo->mutex);
	if (check_stop(philo->params))
		return (pthread_mutex_unlock(&philo->mutex), 1);
	philo->last_meal = get_timestamp();
	pthread_mutex_lock(&philo->params->print_mutex);
	printf("%lu\t%d has taken a fork\n", get_timestamp() - params->start,
		philo->index + 1);
	printf("%lu\t%d has taken a fork\n", get_timestamp() - params->start,
		philo->index + 1);
	printf("%lu\t%d is eating\n", get_timestamp() - params->start,
		philo->index + 1);
	pthread_mutex_unlock(&philo->params->print_mutex);
	pthread_mutex_unlock(&philo->mutex);
	usleep(philo->params->time_to_eat * 1000);
	pthread_mutex_lock(&philo->mutex);
	pthread_mutex_lock(&philo->forks[0]->mutex);
	pthread_mutex_lock(&philo->forks[1]->mutex);
	philo->num_meals++;
	philo->forks[0]->user = -1;
	philo->forks[1]->user = -1;
	pthread_mutex_unlock(&philo->forks[1]->mutex);
	pthread_mutex_unlock(&philo->forks[0]->mutex);
	pthread_mutex_unlock(&philo->mutex);
	return (0);
}

static int	philo_sleep(t_philo *const philo, t_params *const params)
{
	pthread_mutex_lock(&params->mutex);
	if (params->stop)
		return (pthread_mutex_unlock(&params->mutex), 1);
	pthread_mutex_lock(&philo->params->print_mutex);
	printf("%lu\t%d is sleeping\n", get_timestamp() - philo->params->start,
		philo->index + 1);
	pthread_mutex_unlock(&philo->params->print_mutex);
	pthread_mutex_unlock(&params->mutex);
	usleep(philo->params->time_to_sleep * 1000);
	return (0);
}

void	*philosopher(void *arg)
{
	t_philo *const	philo = arg;
	t_params *const	params = philo->params;

	philo_sync(params);
	while (1)
	{
		if (philo_think(philo, params))
			return (NULL);
		while (!check_can_eat(philo, params))
		{
			if (check_stop(params))
				return (NULL);
			usleep(1);
		}
		if (philo_eat(philo, params) || philo_sleep(philo, params))
			return (NULL);
	}
	return (NULL);
}
