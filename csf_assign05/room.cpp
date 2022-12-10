#include <iostream>
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
  // std::set<User *>::iterator it = members.find(user);
  if (members.count(user) <= 0) {
    Guard g(lock);//protect access while adding user
    this->members.insert(user);
    // pthread_mutex_unlock(&lock);
  }
}

void Room::remove_member(User *user) {
  // TODO: remove User from the room
  if (members.count(user) > 0) {
    Guard g(lock);//protect access while adding user
    members.erase(members.find(user));
    // pthread_mutex_unlock(&lock);
  }
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  // send a message to every (receiver) User in the room
  std::set<User *>::iterator it;
  Message msg;
  //format text correctly for the message class
  msg.data = strip_text(get_room_name());
  msg.data += ":";
  msg.data += strip_text(sender_username);
  msg.data += ":";
  msg.data += strip_text(message_text);
  msg.tag = "delivery";
  
  for (it = members.begin(); it != members.end(); ++it) { //loop through everyone in room
    if (!((*it)->is_sender)) { //only send the message to a user if it is NOT a sender
      (*it)->mqueue.enqueue(new Message(msg.tag,msg.data)); // Create new message for each target
    }
  }
  
}

std::string Room::strip_text(std::string input) { //strip text helper function identical to others in other classes
  size_t pos = (input).find("\n");
  if (pos != std::string::npos) {
    input.erase(pos,input.length());
  }
  pos = (input).find("\r");
  if (pos != std::string::npos) {
    input.erase(pos,input.length());
  }
  return input;
}
