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

  Connection conn;
  conn.connect(server_hostname,server_port);

  // Send rlogin 
  std::string login_message = "rlogin:";
  std::string user = argv[3];
  login_message += user;
  conn.send(login_message);
  char response[550];
  bool works = conn.receive(response);
  if (works == false) {
    exit(-1);
  }

  // Join correct room
  std::string join_message = "join:";
  std::string room = argv[4];
  join_message += room;
  conn.send(join_message);
  char join_response[550];
  works = conn.receive(join_response);
  if (works == false) {
    exit(-1);
  }

  // Loop waiting for messages from server (which should be tagged with TAG_DELIVERY)
  bool session_active = true;
  while (session_active)
  {
    // Receive messages and print them
    // Read info into buffer
    char message[550] = "\n";
    conn.receive(message);
    std::string formatted_message(message);
    std::string new_delimiter = ":";
    std::string new_tag = formatted_message.substr(0, formatted_message.find(new_delimiter)); 

    if(new_tag == "delivery") {
      std::string delimiter = ":";
      std::string room;
      std::string sender;
      std::string message;
      
      // Parse
      size_t pos = 0;
      std::string token;
      int i = 0;
      
      while((pos = (formatted_message).find(delimiter)) != std::string::npos) {
          token = (formatted_message).substr(0, pos);
          if(i == 0) {
              i++;
          } else if (i == 1) {
              room = token;
              i++;
          } else if (i == 2) {
              sender = token;
              i++;
          }
          formatted_message.erase(0, pos + delimiter.length());
      }
      if (i!=3 && i!=0) {
        std::cerr << "invlaid format message returned" << std::endl;
        return -1;
      }
      pos = (formatted_message).find("\n");
      if (pos != std::string::npos) {
        formatted_message.erase(pos,formatted_message.length());
      }
      pos = (formatted_message).find("\r");
      if (pos != std::string::npos) {
        formatted_message.erase(pos,formatted_message.length());
      }
      message = formatted_message;
      std::cout << sender << ": " << message << std::endl;
    } 
  }
  return 0;
}
