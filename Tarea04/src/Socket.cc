
#include "Socket.h"

/**
 * @brief Socket constructor
 * @param type Specifies the communication semantics
 * @param ipv6 Indicates if ipv6 is used
 */
Socket::Socket(char type, bool ipv6) {
  this->ipv6 = ipv6;
  this->id = -1;
  this->port = DEFAULT_PORT;
  this->SSLStruct = nullptr;
  this->SSLContext = nullptr;
  if (!this->ipv6)
    this->id = type == 's' ? socket(AF_INET, SOCK_STREAM, 0)
                           : socket(AF_INET, SOCK_DGRAM, 0);
  else
    this->id = type == 's' ? socket(AF_INET6, SOCK_STREAM, 0)
                           : socket(AF_INET6, SOCK_DGRAM, 0);
}

Socket::Socket( int id ) {
  this->id = id;
}

/**
 * @brief Method that closes a socket
 */
Socket::~Socket() {
  this->Close();
}

void Socket::Close(){
  close(this->id);
}

/**
 * @brief Method that connects with a port and an ip address
 * @param hostip Ip address
 * @param port Port number
 * @return int 
 */
int Socket::Connect(const char* hostip, int port) {
  struct sockaddr_in host4;
  memset(reinterpret_cast<char *>(&host4), 0, sizeof(host4));
  host4.sin_family = AF_INET;
  inet_pton(AF_INET, hostip, &host4.sin_addr);
  host4.sin_port = htons(port);
  int st = connect(this->id, reinterpret_cast<sockaddr *>(&host4),
    sizeof(host4));
  if (st == -1) {
    perror("Socket::Connect");
    exit(2);
  }
  return st;
}

/**
 * @brief Method that connects with a port and an ip address
 * @param hostip Ip address
 * @param port Port address
 * @return int 
 */
int Socket::Connect(const char* hostip, const char* port) {
  struct sockaddr_in host4;
  memset(reinterpret_cast<char *>(&host4), 0, sizeof(host4));
  host4.sin_family = AF_INET;
  inet_pton(AF_INET, hostip, &host4.sin_addr);
  host4.sin_port = htons(atoi(port));
  int st = connect(this->id, reinterpret_cast<sockaddr *>(&host4),
           sizeof(host4));
  if (st == -1) {
    perror("Socket::Connect");
    exit(2);
  }
  return st;
}

/**
 * @brief Method that connects with a port and an ip address
 * in IPV6
 * @param hostip Ip address
 * @param port Port address
 * @return int 
 */
int Socket::ConnectIPV6(const char* hostip, int port) {
  struct sockaddr_in6 host6;
  memset(reinterpret_cast<char *>(&host6), 0, sizeof(host6));
  host6.sin6_family = AF_INET6;
  inet_pton(AF_INET6, hostip, &host6.sin6_addr);
  host6.sin6_port = htons(port);
  int st = connect(this->id, reinterpret_cast<sockaddr *>(&host6),
           sizeof(host6));
  if (st == -1) {
    perror("Socket::Connect");
    exit(2);
  }
  return st;
}

/**
 * @brief Method that connects with a port and an ip address
 * in IPV6
 * @param hostip Ip address
 * @param port Port address
 * @return int 
 */
int Socket::ConnectIPV6(const char* hostip, const char* port) {
  struct sockaddr_in6 host6;
  memset(reinterpret_cast<char *>(&host6), 0, sizeof(host6));
  host6.sin6_family = AF_INET6;
  inet_pton(AF_INET6, hostip, &host6.sin6_addr);
  host6.sin6_port = htons(atoi(port));
  int st = connect(this->id, reinterpret_cast<sockaddr *>(&host6),
           sizeof(host6));
  if (st == -1) {
    perror("Socket::Connect");
    exit(2);
  }
  return st;
}

/**
 * @brief Method that reads from a socket
 * @param info Information to read
 * @param length Size of the information vector
 * @return int 
 */
int Socket::Read(char* info, int length) {
  return read(this->id, info, length);
}

/**
 * @brief Method that writes to a socket
 * @param info Information to write
 * @return int 
 */
int Socket::Write(const char* info) {
  return write(this->id, info, strlen(info));
}

int Socket::sendTo(const void* message, int lenght, void* address){
  int bytesSended =  sendto(this->id, message, lenght, 0, (const struct sockaddr*)address, sizeof(sockaddr_in));
  if (bytesSended == -1) {
    perror("Socket::sendTo");
    exit(2);
  }
  printf("Bytes sent: %d", bytesSended);
  return bytesSended;
}

int Socket::recvFrom(void* buffer, int bufferSize, void* address){
  socklen_t addrlen = sizeof(address);
  int bytesReceived = recvfrom(this->id, buffer, bufferSize, 0, (struct sockaddr*)address, &addrlen);
  if (bytesReceived == -1) {
    perror("Socket::recvFrom");
    exit(2);
  }
  printf("Bytes received: %d", bytesReceived);
  return bytesReceived;
}

int Socket::Bind( int port ) {
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl( INADDR_ANY );
  server_addr.sin_port = htons( port );
  size_t len = sizeof( server_addr );

  return bind( this->id, (const sockaddr*) &server_addr, len );
}