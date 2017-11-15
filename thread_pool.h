#ifndef thread_pool_h
#define thread_pool_h
typedef struct thread_pool *thread_pool_t;

thread_pool_t thread_pool_create(unsigned int thread_count);

void thread_pool_add_task(thread_pool_t pool, void* (*routine)(void *arg), void *arg);

void thread_pool_destroy(thread_pool_t pool);

#endif /* thread_pool_h */
