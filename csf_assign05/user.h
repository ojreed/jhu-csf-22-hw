#ifndef USER_H
#define USER_H

#include <string>
#include "message_queue.h"

struct User {
  std::string username;

  // queue of pending messages awaiting delivery
  MessageQueue mqueue;

  bool is_sender;

  User(const std::string &username, bool is_sender) : username(username), is_sender(is_sender){ }
};

#endif // USER_H
