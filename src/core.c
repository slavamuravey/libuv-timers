#include "uv.h"
#include "internal.h"

static int uv__loop_alive(const uv_loop_t* loop) {
  return loop->active_handles > 0;
}

static int uv__backend_timeout(const uv_loop_t* loop) {
  if (loop->active_handles > 0) {
    return uv__next_timeout(loop);
  }
  return 0;
}

int uv_run(uv_loop_t* loop, uv_run_mode mode) {
  int timeout;
  int r;

  r = uv__loop_alive(loop);
  uv__update_time(loop);

  if (r) {
    uv__run_timers(loop);
  }

  while (r) {
    timeout = uv__backend_timeout(loop);
    uv__io_poll(loop, timeout);

    uv__update_time(loop);
    uv__run_timers(loop);

    r = uv__loop_alive(loop);
  }

  return 0;
}
