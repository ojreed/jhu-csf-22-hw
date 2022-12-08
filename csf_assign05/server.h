#ifndef SERVER_H
#define SERVER_H

#include <map>
#include <string>
#include <pthread.h>
#include "user.h"
#include "connection.h"

class Room;

class Server {
public:
  Server(int port);
  ~Server();

  bool listen();

  void chat_with_sender(User *user, int client_fd, Connection *conn);

  void chat_with_receiver(User *user, int client_fd, Connection *conn);

  void handle_client_requests();

  Room *join(User *user, std::string room_name);

  bool sendall(User *user, Room *cur_room,std::string message);

  void sigint_closedown(int sig);

  bool leave(User *user, Room *cur_room);

  bool quit(User *user, Room *cur_room);


  Room *find_or_create_room(const std::string &room_name);

private:
  // prohibit value semantics
  Server(const Server &);
  Server &operator=(const Server &);

  typedef std::map<std::string, Room *> RoomMap;

  // These member variables are sufficient for implementing
  // the server operations
  int m_port;
  int m_ssock;
  RoomMap m_rooms;
  pthread_mutex_t m_lock;
};

#endif // SERVER_H
