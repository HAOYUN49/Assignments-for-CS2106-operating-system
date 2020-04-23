/*************************************
 * Lab 5 Ex2
 * Name: Hao Yun
 * Student No: A0177907N
 * Lab Group: 08
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include "my_stdio.h"

size_t my_fread(void *ptr, size_t size, size_t nmemb, MY_FILE *stream) {
	size_t total = size*nmemb;
  int succ_num = 0;
  int is_eof = 0;
  if ((stream->flags)%8 == 1 || stream->last == 1) return -1;
  if(stream->is_a == 1) lseek(stream->fd, 0, SEEK_SET);
  while (total > 0) {
    if (stream->avai_items >= total) {
      memcpy(ptr, stream->curp, total);
      stream->avai_items -= total;
      stream->curp += total;
      if (stream->avai_items == 0) stream->curp = stream->buff;
      succ_num += total;
      break;
    } else {
      if (stream->avai_items == 0) {
        if (is_eof) break;
        else {
          int num =  read(stream->fd, stream->buff, stream->bsize);
          stream->avai_items += num;
          if (num < stream->bsize) is_eof = 1;
        }
      } else {
        memcpy(ptr, stream->curp, stream->avai_items);
        ptr += stream->avai_items;
        succ_num += stream->avai_items;
        total -= stream->avai_items;
        stream->avai_items = 0;
        stream->curp = stream->buff;
      }
    }
  }
  stream->last = 0;
  return succ_num;
}
