#ifndef UV_UNIX_INTERNAL_H_
#define UV_UNIX_INTERNAL_H_

#include "uv.h"
#include "uv-common.h"

#include <inttypes.h>
#include <time.h>

#if defined(__clang__) ||                                                     \
    defined(__GNUC__) ||                                                      \
    defined(__INTEL_COMPILER)
# define UV_UNUSED(declaration)     __attribute__((unused)) declaration
#else
# define UV_UNUSED(declaration)     declaration
#endif

void uv__io_poll(uv_loop_t* loop, int timeout);

int uv__platform_loop_init(uv_loop_t* loop);

UV_UNUSED(static void uv__update_time(uv_loop_t* loop)) {
  struct timespec t;

  if (clock_gettime(CLOCK_MONOTONIC, &t)) {
    loop->time = 0;
  }

  loop->time = (t.tv_sec * (uint64_t) 1e9 + t.tv_nsec) / 1e6;
}

#endif
