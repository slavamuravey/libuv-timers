#ifndef UV_H
#define UV_H

#include <stdint.h>

/* Internal type, do not use. */
struct uv__queue {
  struct uv__queue* next;
  struct uv__queue* prev;
};

typedef enum {
  UV_RUN_DEFAULT = 0
} uv_run_mode;

/* Handle types. */
typedef struct uv_loop_s uv_loop_t;
typedef struct uv_handle_s uv_handle_t;
typedef struct uv_timer_s uv_timer_t;

struct uv_loop_s {
  /* Loop reference counting. */
  unsigned int active_handles;
  unsigned long flags;                                                        
  int backend_fd;                                                             
  struct {                                                                    
    void* min;                                                                
    unsigned int nelts;                                                       
  } timer_heap;                                                               
  uint64_t timer_counter;  
  uint64_t time;                                                   
};

/* The abstract base class of all handles. */
struct uv_handle_s {
  uv_loop_t* loop;                                                            
  unsigned int flags;                                                    
};

typedef void (*uv_timer_cb)(uv_timer_t* handle);

/*
 * uv_timer_t is a subclass of uv_handle_t.
 *
 * Used to get woken up at a specified time in the future.
 */
struct uv_timer_s {
  uv_loop_t* loop;
  unsigned int flags;
  
  uv_timer_cb timer_cb;                                                      
  union {                                                                    
    void* heap[3];                                                           
    struct uv__queue queue;                                                  
  } node;                                                                    
  uint64_t timeout;                                                          
  uint64_t repeat;                                                           
  uint64_t start_id;
};

int uv_timer_init(uv_loop_t*, uv_timer_t* handle);
int uv_timer_start(uv_timer_t* handle, uv_timer_cb cb, uint64_t timeout, uint64_t repeat);
int uv_timer_stop(uv_timer_t* handle);
int uv_timer_again(uv_timer_t* handle);

int uv_loop_init(uv_loop_t* loop);
int uv_run(uv_loop_t*, uv_run_mode mode);

void uv_unref(uv_handle_t*);

#endif