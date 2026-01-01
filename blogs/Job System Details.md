# Job System

## Main Overview:

* We need a job system to let the engine break the large workloads into smaller units of parallel work that can run on several threads.

### Why do we need?

* A Job Queue, which all N threads would pull tasks, jobs, from and execute them independently. We could implement the queue as lock-based or lock-free. A lock-free queue is faster but more complex to set up.

* A Job Scheduler which would enqueue and dequeue jobs into/out of the Job Queue

* A Job which is a unit of work and in our case, we define as `using JobFn = std::function<void()>;`
  
  
  
  

### The Core Layer should include the classes needed to implement the Job System

* ThreadPool

* JobQueue

* ConditionVariables

* Synchronization primitives

* Atomic wrappers

* Core::Logging::Logger (thread-safe)
