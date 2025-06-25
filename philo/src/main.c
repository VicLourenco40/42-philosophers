/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vde-albu <vde-albu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 12:01:14 by vde-albu          #+#    #+#             */
/*   Updated: 2025/06/25 17:26:30 by vde-albu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

long	get_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

t_status	get_philo_status(t_philo *const philo)
{
	int	status;

	pthread_mutex_lock(&philo->mutex);
	status = philo->status;
	pthread_mutex_unlock(&philo->mutex);
	return (status);
}

void	*philosopher(void *const arg)
{
	t_philo *const	philo = arg;

	while (1)
	{
		pthread_mutex_lock(&philo->mutex);
		philo->status = THINKING;
		printf("%ld %d is thinking\n", get_timestamp(), philo->index);
		pthread_mutex_unlock(&philo->mutex);


		while (get_philo_status(philo) != EATING)
		{
			pthread_mutex_lock(&philo->mutex);
			pthread_mutex_lock(&philo->forks[0]->mutex);
			pthread_mutex_lock(&philo->forks[1]->mutex);
			if (!philo->forks[0]->user && !philo->forks[1]->user)
			{
				philo->forks[0]->user = philo->index;
				philo->forks[1]->user = philo->index;
				philo->status = EATING;
				printf("%ld %d has taken a fork\n", get_timestamp(), philo->index);
				printf("%ld %d has taken a fork\n", get_timestamp(), philo->index);
				printf("%ld %d is eating\n", get_timestamp(), philo->index);
			}
			pthread_mutex_unlock(&philo->forks[0]->mutex);
			pthread_mutex_unlock(&philo->forks[1]->mutex);
			pthread_mutex_unlock(&philo->mutex);
			usleep(1000);
		}
		usleep(philo->params->time_eat * 1000);


		pthread_mutex_lock(&philo->forks[0]->mutex);
		pthread_mutex_lock(&philo->forks[1]->mutex);
		pthread_mutex_lock(&philo->mutex);
		philo->forks[0]->user = 0;
		philo->forks[1]->user = 0;
		philo->num_meals++;
		philo->last_meal = get_timestamp();
		philo->status = SLEEPING;
		printf("%ld %d is sleeping\n", get_timestamp(), philo->index);
		pthread_mutex_unlock(&philo->forks[0]->mutex);
		pthread_mutex_unlock(&philo->forks[1]->mutex);
		pthread_mutex_unlock(&philo->mutex);
		usleep(philo->params->time_sleep * 1000);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_params	params;
	t_philo		*philos;
	t_fork		*forks;
	int			i;

	if (argc < 5)
		return (1);
	params.num_philos = ft_atoi(argv[1]);
	params.time_die = ft_atoi(argv[2]);
	params.time_eat = ft_atoi(argv[3]);
	params.time_sleep = ft_atoi(argv[4]);
	params.min_meals = 0;
	if (argc >= 6)
		params.min_meals = ft_atoi(argv[5]);
	philos = ft_calloc(params.num_philos, sizeof(t_philo));
	if (!philos)
		return (1);
	forks = ft_calloc(params.num_philos, sizeof(t_fork));
	if (!forks)
	{
		free(philos);
		return (1);
	}
	i = -1;
	while (++i < params.num_philos)
	{
		philos[i].index = i + 1;
		philos[i].forks[0] = &forks[i];
		philos[i].forks[1] = &forks[i + 1];
		if (i + 1 >= params.num_philos)
			philos[i].forks[1] = forks;
		philos[i].last_meal = get_timestamp();
		philos[i].params = &params;
		pthread_mutex_init(&philos[i].mutex, NULL);
		pthread_mutex_init(&forks[i].mutex, NULL);
	}
	i = -1;
	while (++i < params.num_philos)
		pthread_create(&philos[i].thread, NULL, philosopher, &philos[i]);
	i = -1;
	while (++i < params.num_philos)
		pthread_join(philos[i].thread, NULL);
	free(philos);
	free(forks);
}
