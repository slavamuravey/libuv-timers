cmake_minimum_required(VERSION 3.10)
project(libuv-timers)
include_directories("include/")
add_compile_definitions(_GNU_SOURCE)

set(
    UV_SOURCES
    
    src/core.c  src/linux.c  src/loop.c  src/timer.c  src/uv-common.c
)

add_executable(
    timers 
    
    ${UV_SOURCES}
    
    examples/timers/main.c
)