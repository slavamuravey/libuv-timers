#include "uv.h"
#include "uv-common.h"
#include "heap-inl.h"

#include <assert.h>
#include <limits.h>

static struct heap *timer_heap(const uv_loop_t* loop) {
  return (struct heap*) &loop->timer_heap;
}

static int timer_less_than(const struct heap_node* ha, const struct heap_node* hb) {
  const uv_timer_t* a;
  const uv_timer_t* b;

  a = container_of(ha, uv_timer_t, node.heap);
  b = container_of(hb, uv_timer_t, node.heap);

  if (a->timeout < b->timeout) {
    return 1;
  }
  if (b->timeout < a->timeout) {
    return 0;
  }

  /* Compare start_id when both have the same timeout. start_id is
   * allocated with loop->timer_counter in uv_timer_start().
   */
  return a->start_id < b->start_id;
}

int uv_timer_init(uv_loop_t* loop, uv_timer_t* handle) {
  ((uv_handle_t*)handle)->loop = loop;                                                      
  ((uv_handle_t*)handle)->flags = UV_HANDLE_REF;  /* Ref the loop when active. */              

  handle->timer_cb = NULL;
  handle->timeout = 0;
  handle->repeat = 0;
  uv__queue_init(&handle->node.queue);
  return 0;
}

int uv_timer_start(uv_timer_t* handle, uv_timer_cb cb, uint64_t timeout, uint64_t repeat) {
  uint64_t clamped_timeout;

  if (cb == NULL) {
    return -1;
  }

  uv_timer_stop(handle);

  clamped_timeout = handle->loop->time + timeout;
  if (clamped_timeout < timeout) {
    clamped_timeout = (uint64_t) -1;
  }

  handle->timer_cb = cb;
  handle->timeout = clamped_timeout;
  handle->repeat = repeat;
  /* start_id is the second index to be compared in timer_less_than() */
  handle->start_id = handle->loop->timer_counter++;

  heap_insert(timer_heap(handle->loop), (struct heap_node*) &handle->node.heap, timer_less_than);
  if (handle->flags & UV_HANDLE_ACTIVE) {
    return 0;          
  }                
  handle->flags |= UV_HANDLE_ACTIVE;                                           
  if (handle->flags & UV_HANDLE_REF) {
    handle->loop->active_handles++;
  }

  return 0;
}

int uv_timer_stop(uv_timer_t* handle) {
  if (handle->flags & UV_HANDLE_ACTIVE) {
    heap_remove(timer_heap(handle->loop), (struct heap_node*) &handle->node.heap, timer_less_than);
    handle->flags &= ~UV_HANDLE_ACTIVE;                                          
    if (handle->flags & UV_HANDLE_REF) {
      handle->loop->active_handles--;        
    }
  } else {
    uv__queue_remove(&handle->node.queue);
  }

  uv__queue_init(&handle->node.queue);
  return 0;
}


int uv_timer_again(uv_timer_t* handle) {
  if (handle->timer_cb == NULL) {
    return -1;
  }

  if (handle->repeat) {
    uv_timer_stop(handle);
    uv_timer_start(handle, handle->timer_cb, handle->repeat, handle->repeat);
  }

  return 0;
}

int uv__next_timeout(const uv_loop_t* loop) {
  const struct heap_node* heap_node;
  const uv_timer_t* handle;
  uint64_t diff;

  heap_node = heap_min(timer_heap(loop));
  if (heap_node == NULL) {
    return -1;
  }

  handle = container_of(heap_node, uv_timer_t, node.heap);
  if (handle->timeout <= loop->time) {
    return 0;
  }

  diff = handle->timeout - loop->time;
  if (diff > INT_MAX) {
    diff = INT_MAX;
  }

  return (int) diff;
}

void uv__run_timers(uv_loop_t* loop) {
  struct heap_node* heap_node;
  uv_timer_t* handle;
  struct uv__queue* queue_node;
  struct uv__queue ready_queue;

  uv__queue_init(&ready_queue);

  for (;;) {
    heap_node = heap_min(timer_heap(loop));
    if (heap_node == NULL) {
      break;
    }

    handle = container_of(heap_node, uv_timer_t, node.heap);
    if (handle->timeout > loop->time) {
      break;
    }

    uv_timer_stop(handle);
    uv__queue_insert_tail(&ready_queue, &handle->node.queue);
  }

  while (!uv__queue_empty(&ready_queue)) {
    queue_node = uv__queue_head(&ready_queue);
    uv__queue_remove(queue_node);
    uv__queue_init(queue_node);
    handle = container_of(queue_node, uv_timer_t, node.queue);

    uv_timer_again(handle);
    handle->timer_cb(handle);
  }
}
