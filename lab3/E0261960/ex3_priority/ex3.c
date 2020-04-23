/*************************************
 * Lab 3 Exercise 3
 * Name: HAO YUN 
 * Student No: A0177907N
 * Lab Group: 08
 *************************************/
 
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#include "traffic_synchronizer.h"

//Using extern, you can use the global variables num_of_cars and num_of_segments from ex3_runner.c in your code.
extern int num_of_cars;
extern int num_of_segments;
extern segment_struct *segments;
sem_t *s;
sem_t can_enter;
pthread_mutex_t *pri;

void initialise()
{
    s = malloc(num_of_segments*sizeof(sem_t));
	pri = malloc(num_of_segments*sizeof(pthread_mutex_t));
    int i;
    for(i = 0; i < num_of_segments; i++) {
        sem_init(&s[i], 0, 1);
		pthread_mutex_init(&pri[i], NULL);
		segments[i].cars_in_seg = 0;
    }
	sem_init(&can_enter, 0, num_of_segments-1);
}

void cleanup()
{
    int i;
    for (i = 0; i < num_of_segments; i++) {
        sem_destroy(&(s[i]));
		pthread_mutex_destroy(&pri[i]);
    }
    free(s);
	free(pri);
	sem_destroy(&can_enter);
}

void* car(void* car)
{
    //This function modeles a thread 
    //A car: 
    //   -should call enter_roundabout (...)
    //   -followed by some calls to move_to_next_segment (...)
    //   -finally call exit_roundabout (...)
    
	car_struct *cars = (car_struct*)car;
    
	sem_wait(&can_enter);
	pthread_mutex_lock(&pri[cars->entry_seg]);
	pthread_mutex_unlock(&pri[cars->entry_seg]);
	sem_wait(&s[cars->entry_seg]);
	enter_roundabout(cars);
    pthread_mutex_lock(&pri[(cars->entry_seg+1)%num_of_segments]);

    while(cars->current_seg != cars->exit_seg) {
        int seg_no = cars->current_seg;
        int next_no = (seg_no+1)%num_of_segments;
        sem_wait(&s[next_no]);
        move_to_next_segment(cars);
        sem_post(&s[seg_no]);
        if(next_no != cars->exit_seg) {
            pthread_mutex_lock(&pri[(next_no+1)%num_of_segments]);
		}
        pthread_mutex_unlock(&pri[next_no]);
    }

    exit_roundabout(cars);
    sem_post(&(can_enter));
    sem_post(&s[cars->exit_seg]);

    pthread_exit(NULL);
}
