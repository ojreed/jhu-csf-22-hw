#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"
#include "csapp.h"

////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////
int NTHREADS;

// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions

struct ConnInfo {
  int clientfd;
  pthread_mutex_t lock;
  volatile int count;
};

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace {

void chat_with_sender(int client_fd) {
  // see sequence diagrams in part 1 for how to implement
  // terminate the loop and tear down the client thread if any message fails to send
  
}

void chat_with_receiver(int client_fd) {
  // terminate the loop and tear down the client thread if any message transmission fails, or if a valid quit message is received
}

void *worker(void *arg) {
   // TODO: use a static cast to convert arg from a void* to
  //       whatever pointer type describes the object(s) needed
  //       to communicate with a client (sender or receiver)
  struct ConnInfo *info = (ConnInfo*) arg;
  pthread_detach(pthread_self());
  for (int i = 0; i < NTHREADS; i++) {
    Guard *g = new Guard(info->lock);
    info->count++;
    delete g;
  }
  // TODO: read login message (should be tagged either with
  //       TAG_SLOGIN or TAG_RLOGIN), send response
  Connection conn;
  conn.connect(server_hostname, server_port); //should this stuff be in ConnInfo too?
  char message[550] = "\n";
  conn.receive(message);
  std::string formatted_message(message);
  std::string delimiter = ":";
  std::string tag = formatted_message.substr(0, formatted_message.find(delimiter)); 

  // TODO: depending on whether the client logged in as a sender or
  //       receiver, communicate with the client (implementing
  //       separate helper functions for each of these possibilities
  //       is a good idea)
  if(tag == "rlogin") {
    chat_with_receiver(info->clientfd);
    close(info->clientfd);
  } else {
    chat_with_sender(info->clientfd);
    close(info->clientfd);
  }

  free(info);

  return nullptr;
}

}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // TODO: initialize mutex 
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL); //pthread_mutex_t *, const pthread_mutexattr_t *_Nullable
}

Server::~Server() {
  // TODO: destroy mutex
}

bool Server::listen() {
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not
  std::string temp_port = std::to_string(this->m_port); // Convert number to a string
  char const* port = temp_port.c_str(); // Convert string to char Array
  if (Open_listenfd(port) < 0) { // param: const char* port
    return false;
  }
  return true;
}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
  // how to deal w serverfd
  int serverfd = open_listenfd((char*) port);
  while(1) {
    int clientfd = Accept(serverfd, NULL, NULL);
    if (clientfd < 0) {
      std::cerr << "Could Not Accept Server\n";
      //close();
      //exit(-1);
    } else {
      // create struct to pass the connection object and 
      // other data to the client thread using the aux parameter
      // of pthread_create
      struct ConnInfo *info = (ConnInfo*) malloc(sizeof(struct ConnInfo)); // or use calloc?
      Server(int x); // need to initialize mutex?
      info->clientfd = clientfd;

      pthread_t threads[NTHREADS];
      for (int i = 0; i < NTHREADS; i++) {
        if(pthread_create(&threads[i], NULL, worker, info) != 0) { // worker() is the thread entrypoint
          std::cerr << "Error\n";
        }
      }
      for (int i = 0; i < NTHREADS; i++) {
        if(pthread_join(threads[i], NULL) != 0) {
          std::cerr << "Error\n";
        }
      }
      //printf("%d\n", obj->count);
      //pthread_mutex_destroy(&obj->lock);
    }
  }
  // Create a user object in each client thread to track the pending messages
  // and register it to a room when client sends join request
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
}
