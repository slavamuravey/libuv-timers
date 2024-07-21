#include <stdio.h>

#include <uv.h>

struct timer {
  uv_timer_t timer;
  uint64_t timeout;
};

void timer_callback(uv_timer_t *handle) {
    printf("timer callback invoked, timeout = %ld\n", handle->timeout);
}

int main() {
  int i;
  uv_loop_t loop;
  struct timer timers[] = {
    { timeout: 6000 }, 
    { timeout: 3000 },
    { timeout: 2000 },
    { timeout: 1000 },
    { timeout: 4000 },
    { timeout: 5000 },
  };

  uv_loop_init(&loop);

  for (i = 0; i < sizeof(timers) / sizeof(*timers); i++) {
    struct timer *timer = &timers[i];
    uv_timer_init(&loop, &timer->timer);
    uv_timer_start(&timer->timer, timer_callback, timer->timeout, 0);
  }
  
  return uv_run(&loop, UV_RUN_DEFAULT);
}
