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
- the mutex protects modification to the set of users in the room at any given time
- all of our mutexes are implementated with guards
- this is a pretty striaght forward case for a mutex where we need a guard aroudn every access to the shared member set 
- we only have one mutex so we dont have to worry about a 2 lock deadlock and it cleanly managed by the guard to ensure unlock

server.cpp (mutexes)
- create_or_find_room function needs a mutex
- this is because mutliple threads can be creating a room at the same time and we need to ensure we dont have a double creation of a room
    - for example if user 1 and 2 join room1 at the same time there is a risk that both trigger the room creation process since the second user might start adding a room before the first finishes 
- this is also implmented with a guard
- since there is no second lock we dont have to worry about a deadlock from the guard
- it cleanly managed by the guard to ensure unlock

message_queue.cpp (mutexes and semaphores)
- enqueue and dequeue fucntions: mutex and semaphore
- we need mutexes (implementated with guards here) in order to ensure that messages are added one at a time and removed cleanly wihtout an interuppting add
- the lock ensures that only one thread can be working with a given recivers message queue at any one time
- the semaphore is used to create a wait while there is no message ready to be pulled out of the queue  
    - when a message is added it will signal to the semaphore wait to let the deuque commence. this leaves the reciver always ready and waiting for a message
    - the time limit that we implemented gauruntees that it is forever halting program and will eventually get out of the lock
- we only have one mutex for the whole queue that ensures whenever the queue comes into scope to be modified there will be a lock in place to ensure only one person modifies it
-since there is no second lock we dont have to worry about a deadlock grom the guard