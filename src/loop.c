#include "uv.h"
#include "internal.h"
#include "heap-inl.h"

int uv_loop_init(uv_loop_t* loop) {
  int err;

  heap_init((struct heap*) &loop->timer_heap);
  uv__queue_init(&loop->handle_queue);

  loop->active_handles = 0;
  uv__update_time(loop);
  loop->backend_fd = -1;

  loop->timer_counter = 0;

  err = uv__platform_loop_init(loop);
  if (err) {
    return err;
  }

  return 0;
}
