/*
 * File for implementation of the message queue (where messages are stored until client can process them).
 * CSF Assignment 5 MS2
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
  sem_init(&m_avail, 0, 0); //init mutex
  pthread_mutex_init(&m_lock, NULL); //init semaphore
}

MessageQueue::~MessageQueue() {
  // destroy the mutex and the semaphore
  pthread_mutex_destroy(&m_lock); //destroy mutex
  sem_destroy(&m_avail); //destory semaphore
  Message *next_message; 
  while((next_message = dequeue())!= nullptr) { //empty the queue and delete all messages
    next_message = NULL;
  }
}

void MessageQueue::enqueue(Message *msg) {
  Guard g(m_lock); //put euqueue inside a lock
  m_messages.push_back(msg);
  // be sure to notify any thread waiting for a message to be
  // available by calling sem_post
  // pthread_mutex_unlock(&m_lock);
  sem_post(&m_avail);
}

Message *MessageQueue::dequeue() {
  struct timespec ts; //track time for semaphore 

  // get the current time using clock_gettime:
  // we don't check the return value because the only reason
  // this call would fail is if we specify a clock that doesn't
  // exist
  clock_gettime(CLOCK_REALTIME, &ts); //get current time

  // compute a time one second in the future
  ts.tv_sec += 1;

  // call sem_timedwait to wait up to 1 second for a message
  //       to be available, return nullptr if no message is available
  if(sem_timedwait(&m_avail, &ts) == 0) { //wait until there is an avail message
    Guard g(m_lock); //lock the queue
    Message *msg = m_messages.front();
    m_messages.pop_front();
    return msg;
  } else {
    return nullptr;
  }
}
