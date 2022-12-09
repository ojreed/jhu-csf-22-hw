Madeline (Maddie) Estey and Owen Reed

We did a combination of individual and group work. Maddie started the server file, then while Owen finished the file, Maddie 
implemented the room and message queue files. Both of us then helped fix bugs in eachothers work and Owen did a lot of testing. Maddie
finished up the style things.

Since synchronization is an important part of this assignment, we’d like you to support a report on your synchronization in your README.txt. 
Please include where your critical sections are, how you determined them, and why you chose the synchronization primitives for each section. 
You should also explain how your critical sections ensure that the synchronization requirements are met without introducing synchronization 
hazards (e.g. race conditions and deadlocks).

Critical sections are sections in code that access a shared resource and so they need to be protected by limiting the number of threads
that can access/modify the section at a given time. Mutexes and semaphores are two ways in which we can protect this sections. Without 
such protection, threads accessing the same resource at the same time might lead to race conditions where the data is modified in ways we
could not predict due to context switching (ex: a counter could have the wrong value if the value is read before another thread is finished incrementing
/decrementing the counter). Deadlocks could also occur. Deadlocks happen when a couple of threads
are blocked indefinitely because they are each waiting for the other to release a lock or finish using a certain resource in order to continue their tasks..

A mutex is a synchronization object that helpd with mutual exclusion. A mutex can be in a locked or unlocked state. One one thread can have a mutex
at a time, all other threads will be blocked (to ensure one thread only modifies/accesses the critical section at a time).

Semaphores are another type of synchronization object that limits the number of threads that can enter a certain section at a time.
The do this by keeping a thread count. Semaphores can be used alongside mutexes

We determined our critical sections by looking at what functions/resources would need to be accessed by different clients (each client is its
own thread).

Critical sections we have:
room.cpp (mutexes)
- add_member and remove_member functions: mutex

message_queue.cpp (mutexes and semaphores)
- enqueue and dequeue fucntions: mutex and semaphore

