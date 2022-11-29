#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
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

  // Listen to port specified
  // In the future put this stuff into connection.cpp functions
  const char * h = server_hostname.c_str();
  std::string temp_str = std::to_string(server_port); // convert number to a string
  char const* server_port2 = temp_str.c_str(); // convert string to char Array
  int fd = Open_clientfd(h, server_port2); 
  rio_t *rp = new rio_t();
  Rio_readinitb(rp, fd);

  // Send rlogin 
  //struct Message login_message = (struct Message) {"rlogin", argv[3]};
  std::string login_message = "rlogin:";
  std::string user = argv[3];
  login_message += user;
  // login_message += "\r\n";
  char const* formatted = login_message.c_str();
  Rio_writen(fd, &formatted, sizeof(formatted));
  struct Message response;
  rio_t rio_response; 
  Rio_readlineb(&rio_response, &response, 225); // Rio_readlineb might be sufficient error-wise actually...
  // Listen for okay from server 
  //if(response.tag == "err") {
    //perror("Error...");
    //exit(-1);
  //}

  // Join correct room
  std::string join_message = "join:";
  std::string room = argv[4];
  join_message += room;
  // join_message += "\r\n";
  char const* formatted_join = join_message.c_str();
  Rio_writen(fd, &formatted_join, 225);

  //Rio_readlineb(&rio_response, &response, 225); // reusing these variables might not be the move, we'll see
  //if(response.tag == "err") {
    //perror("Error...");
    //exit(-1);
  //}

  // Loop waiting for messages from server (which should be tagged with TAG_DELIVERY)
  bool session_active = true;
  while (session_active)
  {
    // Receive messages and print them
    struct Message received;
    rio_t rio_struct; 
    // Read info into buffer
    rio_readlineb(&rio_struct, &received, 225); 

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
      // if the tag in received includes the room and sender
      // then change received.data to received.tag 
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
      message = received.data;
      std::cout << sender << ": " << message << std::endl; 
    } 
  }
  return 0;
}
