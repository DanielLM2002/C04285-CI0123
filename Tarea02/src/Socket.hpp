
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
  int idSocket;
  int port;
  bool ipv6;
  void* SSLStruct;
  void* SSLContext;
  public:
    Socket( char, bool = false );
    Socket( int );
    ~Socket();
    void Close();
    int Connect( const char *, int );
    void InitSSLContext();
    void InitSSL();
    int SSLConnect( char *, int );
    int SSLConnect( char *, char * );
    int SSLRead( void *, int );
    int SSLWrite( void *, int );
  

};

#endif  // !SOCKET_HPP
