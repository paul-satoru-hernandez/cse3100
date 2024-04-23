# Thread Synchronization

## Deadlock

* A state in which no member can make progress
  * All are waiting for other members to take action
  * A member can be a thread, a process, a computer, etc.

### Common solution

* Fixed Order
  * All threads lock mutexes in the same order
* Try and back off
  * Try to lock. If it fails, release all locks and try again.

## Barriers

* Purpose
  * Applications where work is done in "phases"
  * Must have worker threads wait for the entire group to be done before moving to next phase
  * Number of workers know a priority
