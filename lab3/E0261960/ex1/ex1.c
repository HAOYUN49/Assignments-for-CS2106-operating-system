
/*************************************
 * Lab 3 Exercise 1
 * Name: HAO YUN 
 * Student No: A0177907N
 * Lab Group: 08
 *************************************/
#include <pthread.h>
#include "rw_lock.h"

void initialise(rw_lock* lock)
{
  pthread_mutex_init(&(lock->mutex), NULL);
  pthread_mutex_init(&(lock->read_mutex), NULL);
  lock->reader_count = 0;
  lock->writer_count = 0;
}

void writer_acquire(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->mutex));
  lock->writer_count++;
}

void writer_release(rw_lock* lock)
{
  lock->writer_count--;
  pthread_mutex_unlock(&(lock->mutex));
}

void reader_acquire(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->read_mutex));
  if(lock->reader_count == 0) pthread_mutex_lock(&(lock->mutex));
  lock->reader_count++;
  pthread_mutex_unlock(&(lock->read_mutex));
}

void reader_release(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->read_mutex));
  lock->reader_count--;
  if(lock->reader_count == 0) pthread_mutex_unlock(&(lock->mutex));
  pthread_mutex_unlock(&(lock->read_mutex));
}

void cleanup(rw_lock* lock)
{
  pthread_mutex_destroy(&(lock->mutex));
  pthread_mutex_destroy(&(lock->read_mutex));
}
