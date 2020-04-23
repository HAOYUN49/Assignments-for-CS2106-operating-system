/*************************************
 * Lab 5 Ex3
 * Name: HAO YUN
 * Student No: A0177907N
 * Lab Group: 08
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include "my_stdio.h"

size_t my_fwrite(const void *ptr, size_t size, size_t nmemb, MY_FILE *stream) {
	
  size_t total = size*nmemb;
  if((stream->flags)%8 == 0 || stream->last == 0 || stream->last == 2) return -1;

  if(stream->is_a) lseek(stream->fd, 0, SEEK_END);

  while(total > 0) {
    if(stream->empty >= total) {
      memcpy(stream->curp, ptr, total);
      stream->empty -= total;
      stream->curp += total;
      total = 0;
    } else {
      memcpy(stream->curp, ptr, stream->empty);
      ptr += stream->empty;
      total -= stream->empty;
      stream->empty = 0;
    }
    if(stream->empty == 0) {
      write(stream->fd, stream->buff, stream->bsize);
      stream->curp = stream->buff;
      stream->empty = stream->bsize;
    }
  }

  stream->last = 1;
  return size*nmemb;
}
