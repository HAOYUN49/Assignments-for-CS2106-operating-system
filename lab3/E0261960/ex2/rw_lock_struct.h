/*************************************
 * Lab 3 Exercise 2
 * Name: 
 * Student No:
 * Lab Group:
 *************************************/


#include <pthread.h>

typedef struct {
  pthread_mutex_t mutex;
  pthread_mutex_t read_mutex;
  pthread_mutex_t waitQ;
  int reader_count;
  int writer_count;
} rw_lock;

