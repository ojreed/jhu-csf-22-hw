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
  rio_t *rp = new rio_t();
  Rio_readinitb(rp, fd);

  // TODO: send slogin message
  //create login message with tag:payload format 
  std::string login_message = "slogin:";
  std::string user = argv[3];
  login_message += user;
  login_message += "\r\n";
  char const* formatted = login_message.c_str();
  Rio_writen(fd, formatted, strlen(formatted));
  char response[550];
  rio_t rio_response; 
  Rio_readlineb(rp, response, 225); // Rio_readlineb might be sufficient error-wise actually...
  std::string formatted_reply(response);
  std::string delimiter = ":";
  std::string tag = formatted_reply.substr(0, formatted_reply.find(delimiter)); // token is "scott"
  // Listen for okay from server 
  if(tag != "ok") {
    perror(formatted_reply.c_str());
    exit(-1);
  }

  //NOTE: what is the correct way to do size??
  // Rio_writen(fd, &login_message, 225); // send message to server
  //Rio_writen(fd, &login_message, 225); // send message to server <-- idk what this was for lol

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
    //struct Message sender_message;
    std::string sender_message; 
    
    //WHY DOES THIS EXIST
    // char const* formatted = login_message.c_str();
    // Rio_writen(fd, formatted, strlen(formatted));

    if (command_tag == "/join") { //send join
      std::string roomname; 
      roomname = command_ss.str().substr(command_ss.str().find("/join ") + 1); 
      sender_message += "join:";
      sender_message += roomname; 
    } else if (command_tag == "/leave") { //send leave
      sender_message += "leave:";
    } else if (command_tag == "/quit") { // send quit
      sender_message += "quit:";
      // session_active = false; //we want to kill the session after confirmation theoretically 
    } else { //send message
      sender_message = "sendall:";
      sender_message += command_ss.str();
    }
    //Rio_writen(fd, &sender_message, 225); // send message to server
    sender_message += "\r\n";
    char const* formatted_send = sender_message.c_str();
    Rio_writen(fd, formatted_send, strlen(formatted_send)); //new correct way

    //get server response back
    char response[550];
    // rio_t rio_response; THIS IS NOTHING --> THIS IS WHAT I REPLACED WITH RP BELOW TO FIX 
    Rio_readlineb(rp, response, 225); // Rio_readlineb might be sufficient error-wise actually...
    std::string formatted_reply(response);
    std::string delimiter = ":";
    std::string tag = formatted_reply.substr(0, formatted_reply.find(delimiter)); // token is "scott"
    // Listen for okay from server 
    if(tag != "ok") {
      perror(formatted_reply.c_str());
      // exit(-1);
    }
    if ((command_tag == "/quit") && (tag == "ok")) {
      session_active = false;
    }
  }
  return 0; //error?? "expected a declaration"
}
