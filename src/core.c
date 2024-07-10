#include "uv.h"
#include "internal.h"

static int uv__loop_alive(const uv_loop_t* loop) {
  return loop->active_handles > 0;
}

int uv_run(uv_loop_t* loop, uv_run_mode mode) {
  int timeout;
  int has_active_handles;

  has_active_handles = uv__loop_alive(loop);
  if (!has_active_handles) {
    uv__update_time(loop);
  }

  if (has_active_handles) {
    uv__update_time(loop);
    uv__run_timers(loop);
  }

  while (has_active_handles) {
    timeout = uv__next_timeout(loop);
    uv__io_poll(loop, timeout);

    uv__update_time(loop);
    uv__run_timers(loop);

    has_active_handles = uv__loop_alive(loop);
  }

  return 0;
}
