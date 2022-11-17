#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"



int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname;
  int server_port;
  std::string username;

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);
  username = argv[3];

  // TODO: connect to server
  int fd = Open_clientfd(argv[1], argv[2]);

  // TODO: send slogin message
  //create login message with tag:payload format 
  struct Message login_message = {"rlogin",argv[3]};

  //NOTEL what is the correct way to do size??
  Rio_writen(fd, &login_message, 225); // send message to server


  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  bool session_active = true;
  while (session_active)
  {
    std::string command;
    std::getline(std::cin, command);
    std::stringstream command_ss = std::stringstream(command);
    std::string command_tag;
    command_ss >> command_tag;
    struct Message sender_message;
    if (command_tag == "/join") { //send join
      std::string username; 
      command_ss >> username;
      sender_message = {"join",username}; //command SS should contain username
    } else if (command_tag == "/leave") { //send leave
      sender_message = {"leave","IGNORE"};
    } else if (command_tag == "/quit") { // send quit
      sender_message = {"quit","IGNORE"};
    } else { //send message
      sender_message = {"sendall",command};
    }
    Rio_writen(fd, &sender_message, 225); // send message to server
    // std::string result;
    std::string result_tag;
    // Rio_readlineb(fd,&result,225);
    // std::stringstream(result) >> result_tag;
    result_tag = "ok"; //test code NOTE: HOW TO WE GET BACK A MESSAGE
    if ((command_tag == "/quit") && (result_tag == "ok")) {
      session_active = false;
    }
  }
  return 0;
}
