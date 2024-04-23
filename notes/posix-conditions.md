# Threads: POSIX Threads

## Condtion Vairable

* Always paired with a mutex
  * Its access needs to be mutex protected
* If a predicate is not true, a thread can wiat on a dition variable
  * Other threads can signal on the condition vairable 
  * Threads must still check if the preidacte is true due to surprise wake-ups

## Tips

* Mutex protects
  * Shared data and the predicate
* The condition variable
  * lock the mutex before waiting
  * mutex is automatically unlocked while waiting
  * automatically re-lock mutex when waking up

