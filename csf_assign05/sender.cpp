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
  // const char * h = server_hostname.c_str();
  // std::string temp_str = std::to_string(server_port); // convert number to a string
  // char const* server_port2 = temp_str.c_str(); // convert string to char Array
  // int fd = Open_clientfd(h, server_port2);
  // if (fd < 0) {
  //   std::cerr << "Could Not Open Connection\n";
  //   exit(-1);//error message should be printed in Open_client
  // }
  // rio_t *rp = new rio_t();
  // Rio_readinitb(rp, fd);
  Connection conn;
  conn.connect(server_hostname,server_port);

  // TODO: send slogin message
  //create login message with tag:payload format 
  std::string login_message = "slogin:";
  std::string user = argv[3];
  login_message += user;
  conn.send(login_message);
  char response[550];
  bool works = conn.receive(response);
  if (works == false) {
    exit(-1);
  }
  

  //NOTE: what is the correct way to do size??
  // Rio_writen(fd, &login_message, 225); // send message to server
  //Rio_writen(fd, &login_message, 225); // send message to server <-- idk what this was for lol

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
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
    //struct Message sender_message;
    std::string sender_message = ""; 
    
    //WHY DOES THIS EXIST
    // char const* formatted = login_message.c_str();
    // Rio_writen(fd, formatted, strlen(formatted));
    if (command_tag.rfind("/", 0) == 0) {
      if (command_tag == "/join") { //send join
        std::string roomname; 
        roomname = command_ss.str().substr(command_ss.str().find("/join ") + 6); 
        sender_message += "join:";
        sender_message += roomname;
        ready_to_send = true; 
      } else if (command_tag == "/leave") { //send leave
        sender_message += "leave:bye";
        ready_to_send = true; 
      } else if (command_tag == "/quit") { // send quit
        sender_message += "quit:bye";
        ready_to_send = true; 
        // session_active = false; //we want to kill the session after confirmation theoretically 
      } else {
        conn.close();
        std::cerr << "Bad Command Input" << std::endl;
        // exit(-1);
      }
    } else { //send message
      sender_message = "sendall:";
      sender_message += command_ss.str();
      ready_to_send = true; 
    }

    if (ready_to_send) {
      //Rio_writen(fd, &sender_message, 225); // send message to server
      conn.send(sender_message);

      //get server response back
      char response[550];
      
      if (conn.receive(response) && command_tag == "/quit") {
        session_active = false;
      }
    }
  }
  conn.close();
  return 0; //error?? "expected a declaration"
}
