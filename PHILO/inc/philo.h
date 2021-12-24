#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <pthread.h> 
# include <stdlib.h>

typedef struct s_needed
{
	int arguments[6];
	pthread_mutex_t fork[2];
	int eating;
	int sleeping;
	int philosopher;
}	t_needed;

#endif