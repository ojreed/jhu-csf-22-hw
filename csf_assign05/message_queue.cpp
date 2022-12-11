/*
 * File for implementation of the receiver client
 * CSF Assignment 5
 * Madeline Estey (mestey1@jhu.edu)
 * Owen Reed (oreed2@jhu.edu)
 */
#include <cassert>
#include <ctime>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>
#include <iostream>
#include "message_queue.h"
#include "guard.h"

// lock ensures that the message queue can only be modified by one thread at a time, 
// and the semaphore is used to “notify” the other end that a new message is available. 

// no messages available, we want the receiver to sleep until there are available messages, 
// and each time a message is sent, it reduces the available messages by one

// don't directly lock mutexes, use Guard methods

MessageQueue::MessageQueue() {
  // initialize the mutex and the semaphore
  sem_init(&m_avail, 0, 0);
  pthread_mutex_init(&m_lock, NULL);
}

MessageQueue::~MessageQueue() {
  // destroy the mutex and the semaphore
  pthread_mutex_destroy(&m_lock);
  sem_destroy(&m_avail);
  Message *next_message;
  while((next_message = dequeue())!= nullptr) {
    next_message = NULL;
  }
}

void MessageQueue::enqueue(Message *msg) {
  // put the specified message on the queue
  Guard g(m_lock);
  m_messages.push_back(msg);
  // be sure to notify any thread waiting for a message to be
  // available by calling sem_post
  // pthread_mutex_unlock(&m_lock);
  sem_post(&m_avail);
}

Message *MessageQueue::dequeue() {
  struct timespec ts;

  // get the current time using clock_gettime:
  // we don't check the return value because the only reason
  // this call would fail is if we specify a clock that doesn't
  // exist
  clock_gettime(CLOCK_REALTIME, &ts);

  // compute a time one second in the future
  ts.tv_sec += 1;

  //      call sem_timedwait to wait up to 1 second for a message
  //       to be available, return nullptr if no message is available
  if(sem_timedwait(&m_avail, &ts) == 0) {
    Guard g(m_lock);
    // remove the next message from the queue, return it
    Message *msg = m_messages.front();
    m_messages.pop_front();
    return msg;
  } else {
    return nullptr;
  }
}
