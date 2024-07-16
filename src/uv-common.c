#include "uv.h"
#include "uv-common.h"

void uv_unref(uv_handle_t* handle) {
  if ((handle->flags & UV_HANDLE_REF) == 0) {
    return;
  }                       
  
  handle->flags &= ~UV_HANDLE_REF;                                             
  
  if (handle->flags & UV_HANDLE_ACTIVE) {
    handle->loop->active_handles--;
  }
}