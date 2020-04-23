/*************************************
 * Lab 3 Exercise 1
 * Name: HAO YUN
 * Student No: A0177907N
 * Lab Group: 08
 *************************************/


#include <pthread.h>

typedef struct {
  pthread_mutex_t mutex;
  pthread_mutex_t read_mutex;
  int reader_count;
  int writer_count;
} rw_lock;

