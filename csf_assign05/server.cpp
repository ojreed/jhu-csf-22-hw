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
  Server *server;
  volatile int count;
};

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace {

void Server::chat_with_sender(User *user,int client_fd) {
  // see sequence diagrams in part 1 for how to implement
  // terminate the loop and tear down the client thread if any message fails to send
  bool convo_valid = true;
  Connection conn(client_fd);
  Room *cur_room = nullptr;
  while (convo_valid) {
      char message[550];
      conn.receive(message);
      std::string formatted_message(message);
      std::string new_delimiter = ":";
      std::string new_tag = formatted_message.substr(0, formatted_message.find(new_delimiter));
      std::string content = formatted_message.substr(formatted_message.find(new_delimiter) + 1, formatted_message.length()); 
      if (new_tag == "join") {
        //process join room
        cur_room = this->join(user,content);
        conn.send("ok:good join");
      } else if (new_tag == "sendall") {
        //process sendall 
        this->sendall(user,cur_room,content);
        conn.send("ok:it is sent");
      } else if (new_tag == "leave") {
        //process leave
        this->leave(user,cur_room); 
        conn.send("ok:ya gone");
      } else if (new_tag == "quit") {
        //process quit
        this->quit(user,cur_room); 
        conn.send("ok:no whyyyyy");
        convo_valid = false;
      } else {
        std::cerr<<"BAD TAG"<<std::endl;
        this->quit(user,cur_room); 
        conn.send("err:ur bad my guy");
        convo_valid = false;
      }
  }
  conn.close();
}


  Room *Server::join(User *user,std::string room_name) {
    Room *target_room = find_or_create_room(room_name);
    target_room->add_member(user);
    return target_room;
  }

  bool Server::sendall(User *user, Room *cur_room,std::string message) {
    if ((cur_room == nullptr) || (m_rooms.find(cur_room->get_room_name()) != m_rooms.end())) { //checks to see if a room exits in the map
      return false; //if it does we return the room
    } else {
      cur_room->broadcast_message(user->username,message);
      return true;
    }
  }

  bool Server::leave(User *user, Room *cur_room) {
    if ((cur_room == nullptr) || (m_rooms.find(cur_room->get_room_name()) != m_rooms.end())) { //checks to see if a room exits in the map
      return false; //if it does we return the room
    } else {
      cur_room->remove_member(user);
      return true;
    }
  }

  bool Server::quit(User *user, Room *cur_room) { //add any other needed close down code to this
    if ((cur_room == nullptr) || (m_rooms.find(cur_room->get_room_name()) != m_rooms.end())) { //checks to see if a room exits in the map
      cur_room->remove_member(user);
    } 
    return true;
  }

void Server::chat_with_receiver(User *user, int client_fd) {
  // terminate the loop and tear down the client thread if any message transmission fails, or if a valid quit message is received
  bool convo_valid = true;
  Connection conn(client_fd);
  Room *cur_room = nullptr;
  char message[550];
  conn.receive(message);
  std::string formatted_message(message);
  std::string new_delimiter = ":";
  std::string new_tag = formatted_message.substr(0, formatted_message.find(new_delimiter));
  std::string content = formatted_message.substr(formatted_message.find(new_delimiter) + 1, formatted_message.length()); 
  if (new_tag == "join") {
    //process join room
    cur_room = this->join(user,content);
    conn.send("ok:good join");
  } else {
    conn.send("err:bad join tag");
    return;
  } 
  while (convo_valid) {
    Message *message_to_send = user->mqueue.dequeue();
    std::string message_as_string = message_to_send->tag+message_to_send->data;
    convo_valid = conn.send(message_as_string.c_str());
  }
  conn.close();
  this->quit(user,cur_room);
  return;
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
  Connection conn(info->clientfd);
  char message[550] = "\n";
  conn.receive(message);
  std::string formatted_message(message);
  std::string delimiter = ":";
  std::string tag = formatted_message.substr(0, formatted_message.find(delimiter)); 
  std::string username = formatted_message.substr(formatted_message.find(delimiter) + 1, formatted_message.length()); 
  
  // TODO: depending on whether the client logged in as a sender or
  //       receiver, communicate with the client (implementing
  //       separate helper functions for each of these possibilities
  //       is a good idea)
  if(tag == "rlogin") { //parse login message tag for recv
    conn.send("ok:hello"); //message ok because we got a good login
    User user(username,false); //init user as recv
    info->server->chat_with_receiver(&user,info->clientfd); //move into recv loop
    conn.close();
  } else if(tag == "slogin") { //parse login message tag for sender
    conn.send("ok:hello"); //message ok because we got a good login
    User user(username,true); //init user as sender
    info->server->chat_with_sender(&user,info->clientfd); //move into sender loop
    conn.close();
  } else {
    //error?
    std::cerr << "BAD FIRST TAG" << std::endl;
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
  this->m_ssock = Open_listenfd(port);
  if (m_ssock < 0) { // param: const char* port
    return false;
  }
  return true;
}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
  // how to deal w serverfd
  // listen(); //I think this is hadneld in the server main func
  while(1) {
    int clientfd = Accept(m_ssock, NULL, NULL);
    if (clientfd < 0) {
      std::cerr << "Could Not Accept Server\n";
      //close();
      //exit(-1);
    } else {
      // create struct to pass the connection object and 
      // other data to the client thread using the aux parameter
      // of pthread_create
      struct ConnInfo *info = (ConnInfo*) malloc(sizeof(struct ConnInfo)); // or use calloc?
      info->clientfd = clientfd;
      info->server = this;

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
  if (m_rooms.count(room_name)>0) { //checks to see if a room exits in the map
    return m_rooms[room_name]; //if it does we return the room
  } else { //if it doesnt
    Room* new_room = new Room(room_name);
    m_rooms[room_name] = new_room; //create a new room by constructor w/ roomname
    return new_room; //return the new room
  }
}
