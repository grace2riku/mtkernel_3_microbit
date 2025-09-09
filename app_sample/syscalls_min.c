/*
 * syscalls_min.c
 *
 *  Created on: 2025/09/09
 *      Author: k-abe
 */

#include <sys/types.h>
#include <errno.h>

#ifndef NN_HEAP_SIZE
#define NN_HEAP_SIZE (16 * 1024)   // 必要に応じて増やす
#endif

static unsigned char s_heap[NN_HEAP_SIZE];
static unsigned char* s_brk = s_heap;

caddr_t _sbrk(int incr) {
  unsigned char* prev = s_brk;
  if (s_brk + incr > s_heap + NN_HEAP_SIZE) { errno = ENOMEM; return (caddr_t)-1; }
  s_brk += incr;
  return (caddr_t)prev;
}

void _exit(int status){ (void)status; for(;;){} }
int  _write(int fd, const void* buf, unsigned len){ (void)fd; (void)buf; return (int)len; }
int  _read (int fd, void* buf, unsigned len){ (void)fd; (void)buf; return 0; }
int  _close(int fd){ (void)fd; return -1; }
int  _lseek(int fd, int ptr, int dir){ (void)fd;(void)ptr;(void)dir; return 0; }
int  _isatty(int fd){ (void)fd; return 1; }
int  _fstat(int fd, void* st){ (void)fd; (void)st; return 0; }
int  _getpid(void){ return 1; }
int  _kill(int pid, int sig){ (void)pid; (void)sig; return -1; }

