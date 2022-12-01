/*
 * File for implementation of the sender client
 * CSF Assignment 5
 * Madeline Estey (mestey1@jhu.edu)
 * Owen Reed (oreed2@jhu.edu)
 */

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

  // Connect to server
  Connection conn;
  conn.connect(server_hostname,server_port);

  // Login
  std::string login_message = "slogin:";
  std::string user = argv[3];
  login_message += user;
  conn.send(login_message);
  char response[550];
  bool works = conn.receive(response);
  if (works == false) {
    exit(-1);
  }
  
  // Loop reading commands from user, sending messages to server as appropriate
  bool session_active = true;
  bool ready_to_send = false;
  while (session_active)
  {
    ready_to_send = false;
    std::string command;
    std::getline(std::cin, command);
    std::stringstream command_ss = std::stringstream(command);
    std::string command_tag;
    command_ss >> command_tag;
    std::string sender_message = ""; 
    
    if (command_tag.rfind("/", 0) == 0) {
      if (command_tag == "/join") { // Send join
        std::string roomname; 
        roomname = command_ss.str().substr(command_ss.str().find("/join ") + 6); 
        sender_message += "join:";
        sender_message += roomname;
        ready_to_send = true; 
      } else if (command_tag == "/leave") { // Send leave
        sender_message += "leave:bye";
        ready_to_send = true; 
      } else if (command_tag == "/quit") { // Send quit
        sender_message += "quit:bye";
        ready_to_send = true; 
      } else {
        conn.close();
        std::cerr << "Bad Command Input" << std::endl;
      }
    } else { // Send message
      sender_message = "sendall:";
      sender_message += command_ss.str();
      ready_to_send = true; 
    }

    if (ready_to_send) {
      conn.send(sender_message);

      // Get server response back
      char response[550];
      
      if (conn.receive(response) && command_tag == "/quit") {
        session_active = false;
      }
    }
  }
  conn.close();
  return 0; 
}
