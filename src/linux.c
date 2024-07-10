#include "uv.h"
#include "internal.h"

#include <stddef.h>
#include <errno.h>
#include <sys/epoll.h>

void uv__io_poll(uv_loop_t* loop, int timeout) {
  int saved_errno;
  int epollfd;
  int nfds;

  epollfd = loop->backend_fd;

  nfds = epoll_wait(epollfd, NULL, 1, timeout);

  saved_errno = errno;                                                 
  uv__update_time(loop);                                                  
  errno = saved_errno; 
}

int uv__platform_loop_init(uv_loop_t* loop) {
  loop->backend_fd = epoll_create(1);

  if (loop->backend_fd == -1) {
    return -1;
  }

  return 0;
}
