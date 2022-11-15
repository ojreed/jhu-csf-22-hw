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

  //NOTE FROM OWEN: Unsure if this is how any of this works but this is a loose idea of it
  // TODO: connect to server
  int fd = open_clientfd(server_hostname, server_port);
  if (fd < 0) { fatal("Couldn't connect to server"); }

  // TODO: send slogin message
  std::string login_message = strcat("slogin:",username);//create login message with tag:payload format 
  rio_writen(fd, login_message, strlen(argv[3])); // send message to server


  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  bool session_active = true;
  while (session_active)
  {
    std::string command;
    std::getline(std::cin, command);
    std::string command_instruction << command;
    std::string formated_package;
    
    switch (command_instruction)
    {
    case "/join":
    case "/leave":
    case "/quit":
      //send join/leave/quit needs concatonation of specified header with colon
      formated_package = command_instruction;
      command_instruction<<":";
    default:
      //send message
      formated_package = "sendall";
      command_instruction<<":"<<command_instruction; //add removed front back
    }
    formated_package = strcat(formated_package,command);
    //TODO: verify that sizeof(message) makes sence for rio_writen
    Rio_writen(fd,formated_package,sizeof(formated_package)); //should send the package in correct tag:message format
    if (/*CONDITION TO QUIT*/) {
      session_active = false;
    }
  }
  return 0;
}
