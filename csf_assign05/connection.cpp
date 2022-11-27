#include <sstream>
#include <cctype>
#include <cassert>
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
  //int fd = open_clientfd((const char&)hostname, (const char*)port); // Error for some reason on the second (
  // Call rio_readinitb to initialize the rio_t object
  rio_t *rp;
  rio_readinitb(rp, fd);
}

Connection::~Connection() {
  // Close the socket if it is open... by closing file descriptor
}

bool Connection::is_open() const {
  // Return true if the connection is open
}

void Connection::close() {
  // Close the connection if it is open
}

bool Connection::send(const Message &msg) {
  // Send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
}

bool Connection::receive(Message &msg) {
  // Receive a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
}
