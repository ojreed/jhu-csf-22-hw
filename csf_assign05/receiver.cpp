#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];

  Connection conn;

  // listen to port specified
  int fd = Open_listenfd(argv[2]);

  // TODO: send rlogin 
  struct Message login_message = (struct Message) {"rlogin", argv[3]};
  Rio_writen(fd, &login_message, 225);

  // Join correct room
  struct Message join_message = (struct Message) {"join", argv[4]};
  Rio_writen(fd, &join_message, 225);

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  bool session_active = true;
  while (session_active)
  {
    //TODO: receive messages and print them
    struct Message received;
    rio_t rio_struct; // This is definitely set up wrong
    // Read info into buffer
    rio_readlineb(&rio_struct, &received, 255);

    if(received.tag == "delivery") {
      std::string delimiter = ":";
      std::string tag;
      std::string room;
      std::string sender;
      std::string message;
      
      // Parse
      size_t pos = 0;
      std::string token;
      int i = 0;
      while((pos = (received.data).find(delimiter)) != std::string::npos) {
          token = (received.data).substr(0, pos);
          if(i == 0) {
              std::string tag = token;
              i++;
          } else if (i == 1) {
              std::string room = token;
              i++;
          } else {
              std::string sender = token;
          }
          received.data.erase(0, pos + delimiter.length());
      }
      //std::cout << sender << ": " << received.data << std::endl; <-- for testing
    }

    
  }
  return 0;
}
