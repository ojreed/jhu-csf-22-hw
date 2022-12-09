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
  // TODO: send a message to every (receiver) User in the room
  // std::cout << "in broadcast msg func" << std::endl;
  std::set<User *>::iterator it;
  Message msg;
  msg.data = strip_text(get_room_name());
  msg.data += ":";
  msg.data += strip_text(sender_username);
  msg.data += ":";
  msg.data += strip_text(message_text);
  msg.tag = "delivery";
  // std::cout << "message put together:" <<std::endl;
  // std::cout << msg.data <<std::endl;
  
  for (it = members.begin(); it != members.end(); ++it) {
    // Create new message
    if (!((*it)->is_sender)) {
      // std::cout << "adding message to queue" <<std::endl;
      (*it)->mqueue.enqueue(new Message(msg.tag,msg.data));
    }
  }
  
}

std::string Room::strip_text(std::string input) {
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
