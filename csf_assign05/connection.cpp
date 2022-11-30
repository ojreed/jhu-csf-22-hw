#include <sstream>
#include <cctype>
#include <cassert>
#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <regex>
#include "csapp.h"
#include "message.h"
#include "connection.h"

Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}

Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  // Call rio_readinitb to initialize the rio_t object
  rio_t *rp;
  rio_readinitb(rp, fd);
}

void Connection::connect(const std::string &hostname, int port) {
  // Call open_clientfd to connect to the server
  const char * h = hostname.c_str();
  std::string temp_str = std::to_string(port); // convert number to a string
  char const* server_port2 = temp_str.c_str(); // convert string to char Array
  int fd = Open_clientfd(h, server_port2);
  if (fd < 0) {
    std::cerr << "Could Not Open Connection\n";
    exit(-1);//error message should be printed in Open_client
  }
  rio_t *rp = new rio_t();
  Rio_readinitb(rp, fd);
  this->m_fd = fd;
  this->m_fdbuf = *rp;
}

Connection::~Connection() {
  // Close the socket if it is open... by closing file descriptor
  if (is_open()) {  
    close();
  }
}

bool Connection::is_open() const {
  if (this->m_fd == -1) {
    return false;
  }
  return true;
}

void Connection::close() {
  // Close the connection if it is open
  if (is_open()){
    Close(this->m_fd);
    this->m_fd = -1;
  }
}

bool Connection::send(std::string msg) {
  // Send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  msg += "\r\n";
  char const* formatted_send = msg.c_str();
  Rio_writen(this->m_fd, formatted_send, strlen(formatted_send)); //new correct way
  m_last_result = SUCCESS;
  return true;

}

bool Connection::receive(char* msg) {
  // Receive a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  rio_t rio_response; 
  Rio_readlineb(&m_fdbuf, msg, 225); // Rio_readlineb might be sufficient error-wise actually...
  std::string new_msg = std::regex_replace((std::string)msg, std::regex("\n"), "");
  std::string formatted_reply(msg);
  std::string delimiter = ":";
  std::string tag = formatted_reply.substr(0, formatted_reply.find(delimiter)); // token is "scott"
  // Listen for okay from server 
  if(tag == "err") {
    perror(formatted_reply.substr(formatted_reply.find(":") + 1).c_str());
    m_last_result = EOF_OR_ERROR;
    return false;
    // exit(-1);
  }
  m_last_result = SUCCESS;
  return true;
}
