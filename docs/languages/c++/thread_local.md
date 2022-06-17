[toc]

# 线程本地变量

Static memory local to a thread (线程局部静态变量)，同时也可称之为线程特有数据（**TSD: Thread-Specific Data**）或者线程局部存储（**TLS: Thread-Local Storage**）。

```c++
#include <pthread.h>

// 确保无论有多少个线程调用多少次该函数，也只会执行一次由init所指向的由调用者定义的函数。
int pthread_once (pthread_once_t *once_control, void (*init) (void));

// Returns 0 on success, or a positive error number on error
int pthread_key_create (pthread_key_t *key, void (*destructor)(void *));

// Returns 0 on success, or a positive error number on error
int pthread_key_delete (pthread_key_t key);

// Returns 0 on success, or a positive error number on error
int pthread_setspecific (pthread_key_t key, const void *value);

// Returns pointer, or NULL if no thread-specific data is associated with key
void *pthread_getspecific (pthread_key_t key);
```



**只要线程终止时与key关联的值不为NULL，则destructor所指的函数将会自动被调用。**



智能的 __thread的变量声明。