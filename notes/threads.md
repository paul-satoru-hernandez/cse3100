# Threads

- Process is a bundle grouping
  - A virtual address space [memory]
  - A collection of files / sockets [IO]
  - A collection of *concurrent threads*    [execution units]

**How is a thread created?**
- By another thread!    ```pthread_create()```
- It is given a stack to execute and a function to run

**What about the first thread?**
- WHen a process is created, there is a single thread

**How does a thread die?**
- Voluntarily, after completing its task ```pthread_exit()```
- Requested by another thread ```pthread_cancel()```
- When the process dies (along with all threads in it)
  - Any thread calls `exit()`
    - Note that `exit()` is called whem `main()` returns


### The pthreads API
- ANSI/IEEE POSIX 1003.1-1995 standard
- Three types of routines:

<center>

|   Routine Prefix    |            Function             |
| :-----------------: | :-----------------------------: |
|     `pthread_`      |         General pthread         |
|   `pthread_attr_`   |        Thread attributes        |
|   `pthread_mutex`   |              mutex              |
| `pthread_mutexattr` |        Mutex attributes         |
|   `pthread_cond_`   |       Condition variables       |
| `pthread_condattr`  | Conditional variable attributes |
|   `pthread_key_`    |    Thread specific data keys    |

`#include <pthread.h>` and add a '`-pthread`' option to compile

</center>

### Thread Concurrency

- Single-core CPU: timesharing
  - When one thread is waiting for an IO to complete...
  - ... another thread can use the CPU for some computing
  - At any one ponit in time, CPU is used by only ONE thread

- Multi core CPU: true concurrenct
  - MIMD arch: multiple instructions, multiple data
  - Threads can execute in paralle, one on each core
  - OS can still preempt threads
  - Useful when #threads >> #cores.


  