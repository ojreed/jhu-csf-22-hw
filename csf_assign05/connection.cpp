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
  rio_t *rp = new rio_t();
  rio_readinitb(rp, fd);
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
  if (fd < 0) {
    std::cerr << "Could Not Open Connection\n";
    close();
    exit(-1); // Error message should be printed in Open_client
  }
  rio_t *rp = new rio_t();
  Rio_readinitb(rp, fd);
  this->m_fd = fd;
  this->m_fdbuf = *rp;
}

/*
 * Terminate client connection to server by closing socket. No parameters.
 */
Connection::~Connection() {
  // Close the socket if it is open 
  if (is_open()) {  
    close();
  }
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
  Rio_writen(this->m_fd, formatted_send, strlen(formatted_send)); //new correct way
  m_last_result = SUCCESS;
  return true;
}

/*
 * Receive a message over existing connection, storing its tag and data in msg, return true if successful.
 * Parameters: message (msg)
 */
bool Connection::receive(char* msg) {
  Rio_readlineb(&m_fdbuf, msg, 225); 
  // Divide up the received message into parts for evaluation
  std::string formatted_reply(msg); 
  std::string delimiter = ":";
  std::string tag = formatted_reply.substr(0, formatted_reply.find(delimiter)); 
  // Listen for okay (or err) message from server 
  if(tag == "err") {
    std::cerr << (formatted_reply.substr(formatted_reply.find(":") + 1).c_str());
    m_last_result = EOF_OR_ERROR;
    return false;
  }
  m_last_result = SUCCESS;
  return true;
}
