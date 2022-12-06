#ifndef SERVER_H
#define SERVER_H

#include <map>
#include <string>
#include <pthread.h>
class Room;

class Server {
public:
  Server(int port);
  ~Server();

  bool listen();

  void chat_with_sender(int client_fd);

  void chat_with_receiver(int client_fd);

  void handle_client_requests();

  Room *join(User user,std::string room_name);

  void sendall(User user, Room cur_room,std::string message);

  void leave(User user, Room cur_room);

  void quit(User user, Room cur_room);

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
