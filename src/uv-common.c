#include "uv.h"

void uv_unref(uv_handle_t* handle) {
  handle->loop->active_handles--;       
}
