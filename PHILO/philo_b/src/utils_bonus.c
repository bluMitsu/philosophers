#include "../inc/philo_bonus.h"

size_t	ft_checktime(long ref)
{
	struct timeval	current_time;
	long			now;

	gettimeofday (&current_time, NULL);
	now = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (now - ref);
}

void	ft_print(char *str, t_info *z)
{
	struct timeval	current_time;
	long			refin;

	gettimeofday (&current_time, NULL);
	refin = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	sem_wait(z->print);
	sem_wait(z->die);
	printf("%ld %zu %s\n", refin - z->timestart, z->ph, str);
	sem_post(z->die);
	sem_post(z->print);
}

void	ft_isdead(t_info *z)
{
	struct timeval	current_time;
	long			refin;

	gettimeofday (&current_time, NULL);
	refin = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	if (refin - z->options[time_to_die] > 0)
	{
		sem_wait(z->die);
		printf("%ld %zu " "\033[0;45m" "died" "\033[0m" ".\n", refin - z->timestart, z->ph);
		exit(1);
	}
}

void	ft_eat(t_info *z)
{
	long			refin;
	struct timeval	current_time;

	gettimeofday (&current_time, NULL);
	refin = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	ft_print("is" "\033[0;32m" " eating" "\033[0m" ".", z);
	while (1)
	{
		usleep(200);
		ft_isdead(z);
		if (ft_checktime(refin) >= z->options[time_to_eat])
			break ;
	}
	gettimeofday (&current_time, NULL);
	refin = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	z->options[time_to_die] = refin + z->timeincrease;
	z->options[time_to_eat]--;
}

void	ft_sleep(t_info *z)
{
	long			refin;
	struct timeval	current_time;

	gettimeofday (&current_time, NULL);
	refin = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	ft_print("is " "\033[0;37m" "sleeping" "\033[0m" ".", z);
	while (1)
	{
		usleep(200);
		if (ft_checktime(refin) >= z->options[time_to_sleep])
			break ;
	}
	ft_print("is " "\033[0;35m" "thinking" "\033[0m" ".", z);
}
