/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vde-albu <vde-albu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:40:11 by vde-albu          #+#    #+#             */
/*   Updated: 2025/07/03 16:48:09 by vde-albu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <unistd.h>
#include <stdio.h>

static int	check_stop(t_params *const params)
{
	int	stop;

	pthread_mutex_lock(&params->mutex);
	stop = params->stop;
	pthread_mutex_unlock(&params->mutex);
	return (stop);
}

static int	check_can_eat(t_philo *const philo)
{
	int	can_eat;

	if (philo->params->num_philos == 1)
		return (0);
	pthread_mutex_lock(&philo->forks[0]->mutex);
	pthread_mutex_lock(&philo->forks[1]->mutex);
	can_eat = philo->forks[0]->user == philo->index && \
		philo->forks[1]->user == philo->index;
	pthread_mutex_unlock(&philo->forks[1]->mutex);
	pthread_mutex_unlock(&philo->forks[0]->mutex);
	return (can_eat);
}

static void	eat(t_philo *const philo)
{
	pthread_mutex_lock(&philo->mutex);
	if (check_stop(philo->params))
	{
		pthread_mutex_unlock(&philo->mutex);
		return ;
	}
	philo->last_meal = get_timestamp();
	pthread_mutex_unlock(&philo->mutex);
	pthread_mutex_lock(&philo->params->print_mutex);
	printf("%lu %d has taken a fork\n", get_timestamp(), philo->index + 1);
	printf("%lu %d has taken a fork\n", get_timestamp(), philo->index + 1);
	printf("%lu %d is eating\n", get_timestamp(), philo->index + 1);
	pthread_mutex_unlock(&philo->params->print_mutex);
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
}

void	*philosopher(void *arg)
{
	t_philo *const	philo = arg;

	philosopher_sync(philo->params);
	while (1)
	{
		if (check_stop(philo->params))
			return (NULL);
		pthread_mutex_lock(&philo->params->print_mutex);
		printf("%lu %d is thinking\n", get_timestamp(), philo->index + 1);
		pthread_mutex_unlock(&philo->params->print_mutex);
		while (!check_can_eat(philo))
		{
			if (check_stop(philo->params))
				return (NULL);
			usleep(1);
		}
		eat(philo);
		if (check_stop(philo->params))
			return (NULL);
		pthread_mutex_lock(&philo->params->print_mutex);
		printf("%lu %d is sleeping\n", get_timestamp(), philo->index + 1);
		pthread_mutex_unlock(&philo->params->print_mutex);
		usleep(philo->params->time_to_sleep * 1000);
	}
	return (NULL);
}
