/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vde-albu <vde-albu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 12:01:14 by vde-albu          #+#    #+#             */
/*   Updated: 2025/07/07 10:19:11 by vde-albu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <stdlib.h>

static int	init_params(t_params *const params, const int num_args,
	char *const *const args)
{
	params->num_philos = ft_atoi(args[1]);
	params->time_to_die = ft_atoi(args[2]);
	params->time_to_eat = ft_atoi(args[3]);
	params->time_to_sleep = ft_atoi(args[4]);
	params->min_meals = 0;
	if (num_args == 6)
		params->min_meals = ft_atoi(args[5]);
	if (params->num_philos <= 0 || params->time_to_die < 0 || \
		params->time_to_eat < 0 || params->time_to_sleep < 0 || \
		params->min_meals < 0)
	{
		ft_putstr_fd("philo: Invalid parameters\n", 2);
		return (1);
	}
	params->philos_ready = 0;
	params->manager_ready = 0;
	params->stop = 0;
	pthread_mutex_init(&params->mutex, NULL);
	pthread_mutex_init(&params->print_mutex, NULL);
	return (0);
}

static int	init(t_params *const params, t_philo **const philos, \
	const int num_args, char *const *const args)
{
	t_fork	*forks;
	int		i;

	if (init_params(params, num_args, args))
		return (1);
	*philos = ft_calloc(params->num_philos, sizeof(t_philo) + sizeof(t_fork));
	if (!*philos)
	{
		ft_putstr_fd("philo: Initialization error\n", 2);
		return (1);
	}
	forks = (t_fork *)&(*philos)[params->num_philos];
	i = -1;
	while (++i < params->num_philos)
	{
		(*philos)[i].index = i;
		(*philos)[i].forks[i % 2] = &forks[i];
		(*philos)[i].forks[(i + 1) % 2] = &forks[(i + 1) % params->num_philos];
		(*philos)[i].params = params;
		forks[i].user = -1;
		pthread_mutex_init(&(*philos)[i].mutex, NULL);
		pthread_mutex_init(&forks[i].mutex, NULL);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_params	params;
	t_philo		*philos;
	int			i;

	if (argc < 5 || argc > 6)
	{
		ft_putstr_fd("usage: philo <num_philosophers> <time_to_die> "
			"<time_to_eat> <time_to_sleep> [<min_meals>]\n", 1);
		return (0);
	}
	if (init(&params, &philos, argc, argv))
		return (1);
	i = -1;
	while (++i < params.num_philos)
		pthread_create(&philos[i].thread, NULL, philosopher, &philos[i]);
	manager(&params, philos);
	i = -1;
	while (++i < params.num_philos)
		pthread_join(philos[i].thread, NULL);
	free(philos);
}
