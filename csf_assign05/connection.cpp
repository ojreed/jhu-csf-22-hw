/*
 * File for implementation of functions that help clients connect to server
 * CSF Assignment 5
 * Madeline Estey (mestey1@jhu.edu)
 * Owen Reed (oreed2@jhu.edu)
 */

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

/*
 * Default constructor, no parameters.
 */
Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}

/*
 * Constructor that takes file descriptor fd as a parameter. Server
 * uses this when it has accepted a specific connection from a client.
 * Parameters: file descriptor fd
 */
Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  // Initialize the rio_t object
  this->m_fdbuf = new rio_t;
  rio_readinitb(m_fdbuf, fd);
}

/*
 * Allow client to connect to server via a specific port and hostname.
 * Parameters: hostname and port
 */
void Connection::connect(const std::string &hostname, int port) {
  const char * h = hostname.c_str();
  std::string temp_str = std::to_string(port); // Convert number to a string
  char const* server_port2 = temp_str.c_str(); // Convert string to char Array
  int fd; 
  // Call open_clientfd to connect to the server
  if ((fd = open_clientfd(h, server_port2)) < 0) {
	  std::cerr << "Could Not Open Connection\n";
    close();
    exit(-1); // Error message should be printed in Open_client
  }
  this->m_fdbuf = new rio_t;
  Rio_readinitb(m_fdbuf, fd);
  this->m_fd = fd;
}

/*
 * Terminate client connection to server by closing socket. No parameters.
 */
Connection::~Connection() {
  // Close the socket if it is open 
  if (is_open()) {  
    close();
  }
  delete m_fdbuf;
}

/*
 * Check if a client-server connection is open. No parameters.
 */
bool Connection::is_open() const {
  if (this->m_fd == -1) {
    return false;
  }
  return true;
}

/*
 * Close connection if it is open. No parameters.
 */
void Connection::close() {
  if (is_open()){
    Close(this->m_fd);
    this->m_fd = -1;
  }
}

/*
 * Send a message over connection, return true if successful.
 * Parameters: message (msg)
 */
bool Connection::send(std::string msg) {
  msg += "\n";
  char const* formatted_send = msg.c_str();
  ssize_t size = rio_writen(this->m_fd, formatted_send, strlen(formatted_send)); //new correct way
  if (size!=strlen(formatted_send)) {
    m_last_result = EOF_OR_ERROR;
    std::cerr << "Bad send" << std::endl;
    return false;
  }
  m_last_result = SUCCESS;
  return true;
}

/*
 * Receive a message over existing connection, storing its tag and data in msg, return true if successful.
 * Parameters: message (msg)
 */
bool Connection::receive(char* msg) {
  // std::cout << "In receive function" << std::endl;
  if ((rio_readlineb(this->m_fdbuf, msg, 225)) < 0) {
    std::cerr << "Rio_readlineb error" <<std::endl;
    m_last_result = EOF_OR_ERROR;
    return false;
  }
  // std::cout << "Raw Message in receive: " << msg << std::endl;
  // std::cout << "Separating message" << std::endl;
  // Divide up the received message into parts for evaluation
  //strip message of extra chars
  std::string formatted_reply(msg); 
  formatted_reply = strip_text(formatted_reply);
  std::string delimiter = ":";
  std::string tag = formatted_reply.substr(0, formatted_reply.find(delimiter)); 
  // Listen for okay (or err) message from server 
  if(tag == "err") {
    std::cerr << (formatted_reply.substr(formatted_reply.find(":") + 1).c_str());
    m_last_result = EOF_OR_ERROR;
    return false;
  }
  if(tag == formatted_reply) {
    std::cerr << (formatted_reply.substr(formatted_reply.find(":") + 1).c_str());
    m_last_result = INVALID_MSG;
    return false;
  }
  m_last_result = SUCCESS;
  // std::cout << "End of receive function" << std::endl;
  return true;
}

std::string Connection::strip_text(std::string input) {
  size_t pos = (input).find("\n");
  if (pos != std::string::npos) {
    input.erase(pos,input.length());
  }
  pos = (input).find("\r");
  if (pos != std::string::npos) {
    input.erase(pos,input.length());
  }
  return input;
}
