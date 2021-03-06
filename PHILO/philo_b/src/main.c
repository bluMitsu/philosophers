/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bharghaz <bharghaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 22:21:43 by bharghaz          #+#    #+#             */
/*   Updated: 2022/01/25 22:21:43 by bharghaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static void	ft_run(t_info *z)
{
	while (1)
	{
		sem_wait(z->forks);
		ft_print("has " "\033[0;31m" "taken a fork" "\033[0m" ".", z);
		sem_wait(z->forks);
		ft_print("has " "\033[0;31m" "taken a fork" "\033[0m" ".", z);
		ft_eat(z);
		sem_post(z->forks);
		sem_post(z->forks);
		ft_sleep(z);
		if (!z->eattimes)
			break ;
	}
	exit (0);
}

void	function(t_info *z)
{
	struct timeval	current_time;

	if (z->ph != z->totph)
		sem_wait(z->start);
	sem_post(z->start);
	gettimeofday (&current_time, NULL);
	z->timestart = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	z->timedie = z->timestart + z->timeincrease;
	if (z->ph % 2 == 1)
		usleep(1000);
	ft_run(z);
}

void	ft_kill(t_info x)
{
	size_t	i;

	i = 1;
	while (i <= x.totph)
	{
		kill(x.id[i], SIGKILL);
		i++;
	}
}

void	ft_wait(t_info x)
{
	int		status;

	x.ph = 1;
	while (x.ph <= x.totph)
	{
		waitpid(-1, &status, 0);
		if (status != 0)
		{
			ft_kill(x);
			break ;
		}
		x.ph++;
	}
}

int	main(int argc, char **argv)
{
	t_info	x;

	if (!set_data(&x, argv, argc))
		return (1);
	if (ft_onephilo(x))
		return (0);
	ft_init_data(&x);
	x.ph = 1;
	while (x.ph <= x.totph)
	{
		x.id[x.ph] = fork();
		if (x.id[x.ph] == 0)
			function(&x);
		x.ph++;
	}
	ft_wait(x);
	ft_free_info(&x);
	return (0);
}
