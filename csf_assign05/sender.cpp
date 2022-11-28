#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"



/*
OWENS BIG LIST OF ISSUES/TODOS/QUESTIONS
  Message size? how do we know what size to do
  String mangament --> as of rn im sure all our strings are fucked ie
    dont remove / in places or :
    dont add end line char (as mentioned in class)
  How do we get a reply back from the server that a message was recived (ok)
    we need this to ensure we handeld the quit message right?! --> may ask max
*/


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
  //int fd = Open_clientfd(argv[1], argv[2]);
  const char * h = server_hostname.c_str();
  std::string temp_str = std::to_string(server_port); // convert number to a string
  char const* server_port2 = temp_str.c_str(); // convert string to char Array
  int fd = Open_clientfd(h, server_port2);
  rio_t *rp;
  Rio_readinitb(rp, fd);

  // TODO: send slogin message
  //create login message with tag:payload format 
  struct Message login_message;
  login_message.tag = "slogin"; 
  login_message.data = argv[3];

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
      sender_message = (struct Message) {"join", username}; //command SS should contain username
    } else if (command_tag == "/leave") { //send leave
      sender_message = (struct Message) {"leave","IGNORE"};
    } else if (command_tag == "/quit") { // send quit
      sender_message = (struct Message) {"quit", "IGNORE"};
    } else { //send message
      sender_message = (struct Message) {"sendall", command};
    }
    Rio_writen(fd, &sender_message, 225); // send message to server
    // std::string result;
    std::string result_tag;
    // Rio_readlineb(fd,&result,225);
    // std::stringstream(result) >> result_tag;
    result_tag = "ok"; //test code NOTE: HOW TO WE GET BACK A MESSAGE- pretty sure after we rio_writen we need to use a rio read func!
    if ((command_tag == "/quit") && (result_tag == "ok")) {
      session_active = false;
    }
  }
  return 0;
}
