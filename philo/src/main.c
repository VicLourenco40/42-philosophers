/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vde-albu <vde-albu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 12:01:14 by vde-albu          #+#    #+#             */
/*   Updated: 2025/06/27 12:42:16 by vde-albu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void	*philosopher(void *arg)
{
	t_philo *const	philo = arg;
	int				eating;

	while (1)
	{
		printf("%lu %d is thinking\n", get_timestamp(), philo->index);
		eating = 0;
		while (!eating)
		{
			pthread_mutex_lock(&philo->forks[0]->mutex);
			pthread_mutex_lock(&philo->forks[1]->mutex);
			eating = philo->forks[0]->user == philo->index && \
				philo->forks[1]->user == philo->index;
			pthread_mutex_unlock(&philo->forks[1]->mutex);
			pthread_mutex_unlock(&philo->forks[0]->mutex);
		}
		printf("%lu %d is eating\n", get_timestamp(), philo->index);
		usleep(philo->params->time_to_eat * 1000);
		pthread_mutex_lock(&philo->mutex);
		pthread_mutex_lock(&philo->forks[0]->mutex);
		pthread_mutex_lock(&philo->forks[1]->mutex);
		philo->num_meals++;
		philo->last_meal = get_timestamp();
		philo->forks[0]->user = 0;
		philo->forks[1]->user = 0;
		pthread_mutex_unlock(&philo->forks[1]->mutex);
		pthread_mutex_unlock(&philo->forks[0]->mutex);
		pthread_mutex_unlock(&philo->mutex);
		printf("%lu %d is sleeping\n", get_timestamp(), philo->index);
		usleep(philo->params->time_to_sleep * 1000);
	}
	return (NULL);
}

void	manager(t_params *const params, t_philo *const philos)
{
	int	i;

	i = -1;
	while (++i < params->num_philos)
		pthread_create(&philos[i].thread, NULL, philosopher, &philos[i]);

	pthread_mutex_lock(&philos[0].forks[0]->mutex);
	pthread_mutex_lock(&philos[0].forks[1]->mutex);
	philos[0].forks[0]->user = 1;
	philos[0].forks[1]->user = 1;
	pthread_mutex_unlock(&philos[0].forks[1]->mutex);
	pthread_mutex_unlock(&philos[0].forks[0]->mutex);

	i = -1;
	while (++i < params->num_philos)
		pthread_join(philos[i].thread, NULL);
}

void	init_params(t_params *const params, const int num_args,
	char *const *const args)
{
	params->num_philos = ft_atoi(args[1]);
	params->time_to_die = ft_atoi(args[2]);
	params->time_to_eat = ft_atoi(args[3]);
	params->time_to_sleep = ft_atoi(args[4]);
	params->min_meals = 0;
	if (num_args == 6)
		params->min_meals = ft_atoi(args[5]);
	params->stop = 0;
	pthread_mutex_init(&params->mutex, NULL);
}

int	init(t_params *const params, t_philo **const philos, const int num_args, \
	char *const *const args)
{
	t_fork	*forks;
	int		i;

	init_params(params, num_args, args);
	*philos = ft_calloc(params->num_philos, sizeof(t_philo) + sizeof(t_fork));
	if (!*philos)
		return (1);
	forks = (t_fork *)&(*philos)[params->num_philos];
	i = -1;
	while (++i < params->num_philos)
	{
		(*philos)[i].index = i + 1;
		(*philos)[i].forks[i % 2] = &forks[i];
		(*philos)[i].forks[(i + 1) % 2] = &forks[(i + 1) % params->num_philos];
		(*philos)[i].last_meal = get_timestamp();
		(*philos)[i].params = params;
		pthread_mutex_init(&(*philos)[i].mutex, NULL);
		pthread_mutex_init(&forks[i].mutex, NULL);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_params	params;
	t_philo		*philos;

	if (argc < 5 || argc > 6 || init(&params, &philos, argc, argv))
		return (1);
	manager(&params, philos);
	free(philos);
}
