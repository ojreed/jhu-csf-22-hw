/*
 * File for implementation of functions to create a server.
 * CSF Assignment 5 MS2
 * Madeline Estey (mestey1@jhu.edu)
 * Owen Reed (oreed2@jhu.edu)
 */

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

// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions

struct ConnInfo {
  int clientfd;
  Server *server;
};

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace {

/*
 * Entry point for new threads. Figure out whether we are talking with a sender or receiver.
 */
void *worker(void *arg) {
  int detach_result = pthread_detach(pthread_self());
  if (detach_result != 0) { // Error
    std::cerr << "Error detaching thread: " << detach_result << std::endl;
  }
  
  // Convert arg to correct type
  struct ConnInfo *info = (ConnInfo*) arg;

  // Read login message 
  Connection conn((info->clientfd));
  char message[550] = "FAILED:To Send";
  bool receive_result = conn.receive(message);
  if (receive_result == false) { // Error
    std::cerr << "Error receiving message from client" << std::endl;
    conn.close();
    delete info;
    return NULL;
  }
  
  // Format message
  std::string formatted_message(message);
  std::string delimiter = ":";
  std::string tag = formatted_message.substr(0, formatted_message.find(delimiter)); 
  std::string username = formatted_message.substr(formatted_message.find(delimiter) + 1, formatted_message.length()); 
  
  // Depending on whether the client logged in as a sender or
  // receiver, communicate with the client 
  if(tag == "rlogin") { // Client is a receiver
    if (!conn.send("ok:hello")) { // Error on login
      std::cerr << "Error sending message to client recv" << std::endl;
      conn.close();
      delete info;
      return NULL;
    }
    User user(username,false); // Init user as recv
    info->server->chat_with_receiver(&user,info->clientfd,&conn); // Move to the receiver communication function
  } else if(tag == "slogin") { // Client is a sneder
    if (!conn.send("ok:hello")) { // Error
      std::cerr << "Error sending message to client sender" << std::endl;
      conn.close();
      delete info;
      return NULL;
    }
    User user(username,true); // Init user as sender
    info->server->chat_with_sender(&user,info->clientfd,&conn); // Move into receiver communication function
  } else { // Error
    conn.send("err:bad_login");
    std::cerr << "BAD FIRST TAG: " << tag << std::endl;
    conn.close();
    delete info;
    return NULL;
  }
  conn.close();
  delete info;
  return NULL;
}

}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

/*
 * Server constructor.
 */
Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // Initialize mutex 
  pthread_mutex_init(&m_lock, NULL); 
}

/*
 * Destructor for server object.
 */
Server::~Server() {
  // Destroy mutex
  pthread_mutex_destroy(&m_lock);
  close(m_ssock);
}

/*
 * Connect to specified server and port when there is a conenction request.
 */
bool Server::listen() {
  // Use open_listenfd to create the server socket
  std::string temp_port = std::to_string(this->m_port); // Convert number to a string
  char const* port = temp_port.c_str(); // Convert string to char Array
  this->m_ssock = open_listenfd(port);
  if (m_ssock < 0) { 
    return false;
  }
  return true;
}

/*
 * Process client requests, create new threads for connections.
 */
void Server::handle_client_requests() {

  // Infinite loop calling accept or Accept, starting a new pthread for each connected client
  while(1) { 
    int clientfd = Accept(m_ssock, NULL, NULL);
    if (clientfd < 0) {
      std::cerr << "Could Not Accept Server\n";
    } else {
      // Create struct to pass the connection object and other data to the client thread using the aux parameter
      // of pthread_create
      struct ConnInfo *info = new ConnInfo();
      info->clientfd = clientfd;
      info->server = this;

      // Start new thread to handle client connection 
      pthread_t thr_id;
      if (pthread_create(&thr_id, NULL, worker, info) != 0) {
        std::cerr << "Error\n";
        delete info;
      }
    }
  }
}

/*
 * Return a pointer to the unique Room object representing
 * the named chat room, creating a new one if necessary.
 */
Room *Server::find_or_create_room(const std::string &room_name) {
  // Check to see if a room exits in the map
  if (m_rooms.count(room_name)>0) { 
    // Return the room if so
    return m_rooms[room_name]; 
  } else { 
    // Create new room
    Room* new_room = new Room(room_name);
    Guard g(m_lock);
    m_rooms[room_name] = new_room; 
    return new_room; 
  }
}

/*
 * Function that communicates with a sender.
 */
void Server::chat_with_sender(User *user, int client_fd, Connection* conn) {
  bool convo_valid = true;
  Room *cur_room = nullptr;
  while (convo_valid) { 
    char message[550]; 
    conn->receive(message);
    
    // Format and split message
    std::string formatted_message(message);
    std::string new_delimiter = ":";
    std::string new_tag = formatted_message.substr(0, formatted_message.find(new_delimiter));
    std::string content = formatted_message.substr(formatted_message.find(new_delimiter) + 1, formatted_message.length()); 
    
    // Process tag
    if (new_tag == "join") { 
      // Leave current room before we join another
      this->leave(user,cur_room); 
      cur_room = nullptr; 

      // Join new room
      cur_room = this->join(user,content); 
      if (cur_room != nullptr) {
        conn->send("ok:good join");
      } else {
        conn->send("err:failed to join");
      }
    } else if (new_tag == "sendall") {
      // Send messages to all user in a room
      bool success = this->sendall(user,cur_room,content);
      if (success) {
        conn->send("ok:it is sent");
      } else {
        conn->send("err:failed to send");
      }
    } else if (new_tag == "leave") {
      // Leave room
      bool success = this->leave(user,cur_room); 
      if (success) {
        cur_room = nullptr;
        conn->send("ok:ya gone");
      } else {
        conn->send("err:not in a room");
      }
    } else if (new_tag == "quit") {
      // Quit
      bool success = this->quit(user,cur_room); 
      if (success) {
        cur_room = nullptr;
        conn->send("ok:no whyyyyy");
        convo_valid = false;
      } else {
        conn->send("err:quit failed");
      }
    } else {
      std::cerr<<"BAD TAG"<<std::endl;
      this->quit(user,cur_room); 
      conn->send("err:ur bad my guy");
      convo_valid = false;
    }
  }
  conn->close(); 
}

/*
 * Add a certain user to their desired room.
 */
Room *Server::join(User *user,std::string room_name) {
  Room *target_room = find_or_create_room(room_name);
  target_room->add_member(user);
  return target_room;
}

/*
 * Send message to all users in specified room.
 */
bool Server::sendall(User *user, Room *cur_room,std::string message) {
  // Check to see if room exists in a map
  if ((cur_room == nullptr) || (m_rooms.count(cur_room->get_room_name()) <= 0)) { 
    return false; 
  } else {
    // Broadcast message
    cur_room->broadcast_message(user->username,message);
    return true;
  }
}

/*
 * Function that allows user to leave a room.
 */
bool Server::leave(User *user, Room *cur_room) {
  // Check to see if room exists in a map
  if ((cur_room == nullptr) || (m_rooms.count(cur_room->get_room_name()) <= 0)) { 
    return false; 
  } else {
    cur_room->remove_member(user);
    return true;
  }
}

/*
 * Quit function (user leaves room).
 */
bool Server::quit(User *user, Room *cur_room) { 
  leave(user,cur_room);
  delete user;
  return true;
}

/*
 * Function to handle talking with receiver
 */
void Server::chat_with_receiver(User *user, int client_fd, Connection* conn) {
  bool convo_valid = true;
  Room *cur_room = nullptr;
  char message[550];
  conn->receive(message);

  // Process message
  std::string formatted_message(message);
  std::string new_delimiter = ":";
  std::string new_tag = formatted_message.substr(0, formatted_message.find(new_delimiter));
  std::string content = formatted_message.substr(formatted_message.find(new_delimiter) + 1, formatted_message.length()); 

  // Logic to join new room
  if (new_tag == "join") {
    cur_room = this->join(user,content);
    conn->send("ok:good join");
  } else {
    conn->send("err:bad join tag");
    conn->close();
    this->quit(user,cur_room);
    return;
  } 

  // Keep trying to read messages from the queue
  while (convo_valid) {
    Message *message_to_send = user->mqueue.dequeue();
    if (message_to_send != nullptr) {
      std::string message_as_string = message_to_send->tag+":"+message_to_send->data;
      delete message_to_send;
      convo_valid = conn->send(message_as_string.c_str());
    }
  }
  conn->close();
  this->quit(user,cur_room);
  return;
}




