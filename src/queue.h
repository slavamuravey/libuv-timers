#ifndef QUEUE_H_
#define QUEUE_H_

#include <stddef.h>

static inline void uv__queue_init(struct uv__queue* q) {
  q->next = q;
  q->prev = q;
}

static inline int uv__queue_empty(const struct uv__queue* q) {
  return q == q->next;
}

static inline struct uv__queue* uv__queue_head(const struct uv__queue* q) {
  return q->next;
}

static inline void uv__queue_insert_tail(struct uv__queue* h, struct uv__queue* q) {
  q->next = h;
  q->prev = h->prev;
  q->prev->next = q;
  h->prev = q;
}

static inline void uv__queue_remove(struct uv__queue* q) {
  q->prev->next = q->next;
  q->next->prev = q->prev;
}

#endif /* QUEUE_H_ */
