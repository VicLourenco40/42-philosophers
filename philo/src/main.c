/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vde-albu <vde-albu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 12:01:14 by vde-albu          #+#    #+#             */
/*   Updated: 2025/06/26 14:37:05 by vde-albu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void	*philosopher(void *arg)
{
	t_philo *const	philo = arg;

	return (NULL);
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

int	init_state(t_state *const state, const int num_args,
	char *const *const args)
{
	t_params *const	params = &state->params;
	int				i;

	init_params(params, num_args, args);
	state->philos = ft_calloc(params->num_philos, \
		sizeof(t_philo) + sizeof(t_fork));
	if (!state->philos)
		return (1);
	state->forks = (t_fork *)&state->philos[params->num_philos];
	i = -1;
	while (++i < params->num_philos)
	{
		state->philos[i].index = i + 1;
		state->philos[i].forks[0] = &state->forks[i];
		state->philos[i].forks[1] = &state->forks[i + 1];
		if (i + 1 == params->num_philos)
			state->philos[i].forks[1] = state->forks;
		state->philos[i].last_meal = get_timestamp();
		state->philos[i].params = params;
		pthread_mutex_init(&state->philos[i].mutex, NULL);
		pthread_mutex_init(&state->forks[i].mutex, NULL);
		pthread_create(&state->philos[i].thread, NULL,
			philosopher, &state->philos[i]);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_state	state;

	if (argc < 5 || argc > 6 || init_state(&state, argc, argv))
		return (1);
	free(state.philos);
}
