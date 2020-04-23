/*************************************
 * Lab 5 Ex4
 * Name: Hao Yun
 * Student No: A0177907N
 * Lab Group: 08
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include "my_stdio.h"

int my_fflush(MY_FILE *stream) {
	
  if(stream->avai_items > 0) {
    stream->avai_items = 0;
  } else if(stream->empty < stream->bsize) {
    write(stream->fd, stream->buff, (stream->bsize - stream->empty));
    stream->empty = stream->bsize;
  }
  stream->curp = stream->buff;
  if(stream->last != 3) stream->last = 2;
  return 0;
}

int my_fseek(MY_FILE *stream, long offset, int whence) {
	off_t new_pos;
  if(stream->empty < stream->bsize){
    write(stream->fd, stream->buff, (stream->bsize - stream->empty));
  }
  new_pos = lseek(stream->fd, offset, whence);
  stream->avai_items = 0;
  stream->empty = stream->bsize;
  stream->curp = stream->buff;
  stream->last = 3;
  return new_pos;
}
