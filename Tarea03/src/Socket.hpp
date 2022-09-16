#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <iostream>

#define DEFAULT_PORT 8080;

class Socket {
 private:
  int id;
  int port;
  bool ipv6;
  void* SSLStruct;
  void* SSLContext;
 
 public:
  explicit Socket(char, bool);
  Socket(int);
  ~Socket();
  int Connect(const char*, int);
  int Connect(const char*, const char*);
  void Close();
  void InitSSL();
  void InitSSLContext();
  int SSLRead(void*, int);
  int SSLWrite(void*, int);
	int SSLConnect(const char*, int);
	int SSLConnect(const char*, char*);
};

#endif  // !SOCKET_HPP