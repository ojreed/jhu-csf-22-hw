#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  // TODO: initialize the mutex
  pthread_mutex_init(&lock, NULL);
  //Guard *og = new Guard(lock);
}

Room::~Room() {
  // TODO: destroy the mutex
  //delete og;
  pthread_mutex_destroy(&lock);
}

void Room::add_member(User *user) {
  // TODO: add User to the room
  std::set<User *>::iterator it = members.find(user);
  if (it != members.end()) {
    // Element was found
  } else {
    Guard *g = new Guard(this->lock); //protect access while adding user
    this->members.insert(user);
    delete g;
  }
}

void Room::remove_member(User *user) {
  // TODO: remove User from the room
  std::set<User *>::iterator it = members.find(user);
  members.erase(it);
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  // TODO: send a message to every (receiver) User in the room
  std::set<User *>::iterator it;
  for (it = members.begin(); it != members.end(); ++it) {
    // Create new message
    (*it)->mqueue.push(will put message here);
  }
  
}
