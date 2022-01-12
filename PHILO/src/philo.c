#include "../inc/philo.h"
// #include <bits/types/struct_timeval.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>


int get_time_passed(t_needed *life, struct timeval *time)
{
	long long int ending_time;
	long long int starting_time;
	struct timeval temp;

	starting_time = (life->starting_time.tv_sec * 1000) + (life->starting_time.tv_usec / 1000);
	if (time)
		ending_time = (time->tv_sec * 1000) + (time->tv_usec / 1000);
	else 
	{
		gettimeofday(&temp, NULL);
		ending_time = (temp.tv_sec * 1000) + (temp.tv_usec / 1000);
	}
	return (ending_time - starting_time);
}

long int since_last_meal(t_needed *life)
{
	long int current;
	long int meal_time;

	meal_time = get_time_passed(life, &life->last_meal);
	current = get_time_passed(life, NULL);
	return (current - meal_time);
}

t_needed *life_init(int philosopher, int *someone_died, struct timeval starting_time)
{
	t_needed 	*sample;
	int 		i;

	i = 1;
	sample = malloc(sizeof(t_needed));
	// while(--argc)
	// {	
	// 	sample->arguments[i - 1] = atoi(argv[i]); // careful !!! not my atoi
	// 	i++;
	// }
	pthread_mutex_init(&sample->is_eating, NULL);
	sample->starting_time = starting_time;
	sample->last_meal = starting_time;
	sample->philosopher = philosopher;
	sample->arguments[i - 1] = -1;
	// sample->sleeping = 0;
	// sample->eating = 0;
	sample->death = 1500;
	return (sample);
}

void *ded(void *args)
{
	t_kitchen *life;

	life = args;
	while(*(life->someone_died))
	{
		pthread_mutex_lock(&life->is_eating); 
		if(since_last_meal(life) >= life->death && *(life->someone_died))
		{	
			*(life->someone_died) = 0; 
			printf("%d\t%d " "\033[0;45m" "someone_died" "\033[0m" ".\n",
				get_time_passed(life, NULL), life->philosopher);
		}
		pthread_mutex_unlock(&life->is_eating); 
	}
	return (NULL);
}

void *test(void *args)
{
	t_kitchen *life;
	int sleeping;
	int eating;
	t_kitchen *mutex[3];

	sleeping = 1;
	eating = 4;
	life = args;
	mutex[0] = get_next_even(life);
	mutex[1] = get_next_odd(life);
	while(*(life->philosopher->someone_died))
	{
		pthread_mutex_lock(&mutex[0]->fork);
		mutex[0]->free = 0;
		pthread_mutex_lock(&mutex[1]->fork);
		mutex[1]->free = 0;
		pthread_mutex_lock(&life->philosopher->is_eating);
		if(*(life->philosopher->someone_died))
		{
			printf("%d\t%d has " "\033[0;31m" "taken a fork" "\033[0m" ".\n",get_time_passed(life->philosopher, NULL), life->philosopher->philosopher);
			printf("%d\t%d has " "\033[0;31m" "taken a fork" "\033[0m" ".\n",get_time_passed(life->philosopher, NULL), life->philosopher->philosopher);
			printf("%d\t%d is" "\033[0;32m" " eating" "\033[0m" ".\n",get_time_passed(life->philosopher, NULL), life->philosopher->philosopher); 
			usleep(eating * 1000);
			gettimeofday(&(life->philosopher->last_meal), NULL);
			pthread_mutex_unlock(&life->philosopher->is_eating);
		}
		pthread_mutex_unlock(&mutex[0]->fork);
		mutex[0]->free = 1;
		pthread_mutex_unlock(&mutex[1]->fork);
		mutex[1]->free = 1;
		if(*(life->philosopher->someone_died))
		{
			printf("%d\t%d is " "\033[0;37m" "sleeping" "\033[0m" ".\n",get_time_passed(life->philosopher, NULL), life->philosopher->philosopher);
			usleep(sleeping * 10);
		}
		if(*(life->philosopher->someone_died))
			printf("%d\t%d is " "\033[0;35m" "thinking" "\033[0m" ".\n",get_time_passed(life->philosopher, NULL), life->philosopher->philosopher);
	}
	return(NULL);
}

int main(int argc, char *argv[])
{
	struct timeval starting_time;
	int philosopher;
	t_kitchen *kitchen;
	int *args;
	int someone_died = 1;

	args = parsed(argc, argv);
	philosopher = 0;
	gettimeofday(&starting_time, NULL);
	while(philosopher++ < args[num_of_philosophers])
	{	
		new_philosopher(philosopher, &someone_died, &kitchen, starting_time); 
		pthread_create(&kitchen->philosopher->philo_id, NULL, &test, kitchen);
		pthread_create(&kitchen->philosopher->death_id, NULL, &ded, kitchen);
	}

	philosopher = 0;
	while(++philosopher < philos)
	{	
		pthread_join(id[philosopher - 1], NULL);
		pthread_detach(idd[philosopher - 1]);
	}
	free(args);
	// free philosophers list
	return (0);
}