#include "uv.h"
#include "queue.h"

enum {
  UV_HANDLE_ACTIVE = 0x00000004,
  UV_HANDLE_REF    = 0x00000008
};

#define container_of(ptr, type, member) \
  ((type *) ((char *) (ptr) - offsetof(type, member)))

int uv__next_timeout(const uv_loop_t* loop);
void uv__run_timers(uv_loop_t* loop);
