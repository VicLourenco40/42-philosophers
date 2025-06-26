/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vde-albu <vde-albu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 12:00:45 by vde-albu          #+#    #+#             */
/*   Updated: 2025/06/26 14:07:31 by vde-albu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <pthread.h>
# include <stddef.h>

typedef struct s_params
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				min_meals;
	int				stop;
	pthread_mutex_t	mutex;
}	t_params;

typedef struct s_fork
{
	int				taken;
	pthread_mutex_t	mutex;
}	t_fork;

typedef enum e_status
{
	THINKING,
	EATING,
	SLEEPING
}	t_status;

typedef struct s_philo
{
	int				index;
	t_status		status;
	t_fork			*forks[2];
	int				num_meals;
	long			last_meal;
	pthread_mutex_t	mutex;
	pthread_t		thread;
	t_params		*params;
}	t_philo;

typedef struct s_state
{
	t_params	params;
	t_philo		*philos;
	t_fork		*forks;
}	t_state;

void	*ft_calloc(size_t nmemb, size_t size);
int		ft_atoi(const char *nptr);
long	get_timestamp(void);

#endif
