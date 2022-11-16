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
  int fd = Open_clientfd(argv[1], argv[2]);

  // TODO: send slogin message
  const std::string login_message = strcat("slogin:",argv[3]);//create login message with tag:payload format 
  Rio_writen(fd, &login_message, strlen(argv[3])); // send message to server


  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  bool session_active = true;
  while (session_active)
  {
    std::string command;
    std::getline(std::cin, command);
    std::string command_instruction;
    std::stringstream(command) >> command_instruction;
    std::string formated_package;
    
    if ((command_instruction == "/join") || (command_instruction == "/leave") || (command_instruction == "/quit")){
      //send join/leave/quit needs concatonation of specified header with colon
      formated_package = command_instruction;
      formated_package += ":";
    } else {
      //send message
      formated_package = "sendall";
      formated_package +=":";
      formated_package += command_instruction; //add removed front back
    }
    //TODO: verify that sizeof(message) makes sence for rio_writen
    formated_package += command;
    Rio_writen(fd,&formated_package,sizeof(formated_package)); //should send the package in correct tag:message format
    std::string result;
    std::string result_tag;
    Rio_readn(fd,&result,10);
    std::stringstream(result) >> result_tag;
    if ((command_instruction == "quit") && (result_tag == "ok")) {
      session_active = false;
    }
  }
  return 0;
}
