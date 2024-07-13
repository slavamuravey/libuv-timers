#include <stdio.h>

#include <uv.h>

uv_timer_t timer3;

void timer3_callback(uv_timer_t *handle);

void timer1_callback(uv_timer_t *handle) {
    fprintf(stdout, "timer1 callback invoked\n");
}

void timer2_callback(uv_timer_t *handle) {
    uv_timer_init(handle->loop, &timer3);
    uv_timer_start(&timer3, timer3_callback, 1000, 0);
    fprintf(stdout, "timer2 callback invoked\n");
}

void timer3_callback(uv_timer_t *handle) {
    fprintf(stdout, "timer3 callback invoked\n");
}

int main() {
    uv_loop_t loop;
    uv_timer_t timer1;
    uv_timer_t timer2;

    uv_loop_init(&loop);

    uv_timer_init(&loop, &timer1);
    uv_unref((uv_handle_t*) &timer1);

    uv_timer_start(&timer1, timer1_callback, 0, 2000);

    uv_timer_init(&loop, &timer2);
    uv_timer_start(&timer2, timer2_callback, 9000, 0);
    
    return uv_run(&loop, UV_RUN_DEFAULT);
}
