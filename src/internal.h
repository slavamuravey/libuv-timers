#ifndef UV_UNIX_INTERNAL_H_
#define UV_UNIX_INTERNAL_H_

#include "uv.h"
#include "uv-common.h"

#include <inttypes.h>
#include <time.h>

void uv__io_poll(uv_loop_t* loop, int timeout);

int uv__platform_loop_init(uv_loop_t* loop);

static void uv__update_time(uv_loop_t* loop) {
  struct timespec t;
  clock_t clock_id;

  clock_id = CLOCK_MONOTONIC;

  if (clock_gettime(clock_id, &t)) {
    loop->time = 0;
  }

  loop->time = (t.tv_sec * (uint64_t) 1e9 + t.tv_nsec) / 1000000;
}

#endif
