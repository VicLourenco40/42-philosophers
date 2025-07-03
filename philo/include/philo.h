/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vde-albu <vde-albu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 12:00:45 by vde-albu          #+#    #+#             */
/*   Updated: 2025/07/03 15:19:54 by vde-albu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stddef.h>

typedef struct s_params
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				min_meals;
	int				philos_ready;
	int				manager_ready;
	int				stop;
	pthread_mutex_t	mutex;
	pthread_mutex_t	print_mutex;
}	t_params;

typedef struct s_fork
{
	int				user;
	pthread_mutex_t	mutex;
}	t_fork;

typedef struct s_philo
{
	int				index;
	t_fork			*forks[2];
	int				num_meals;
	long			last_meal;
	pthread_mutex_t	mutex;
	pthread_t		thread;
	t_params		*params;
}	t_philo;

void	manager(t_params *const params, t_philo *const philos);
void	*philosopher(void *arg);
void	manager_sync(t_params *const params);
void	philosopher_sync(t_philo *const philo);
long	get_timestamp(void);
void	*ft_calloc(size_t nmemb, size_t size);
int		ft_atoi(const char *nptr);
void	ft_putstr_fd(const char *const s, const int fd);

#endif
