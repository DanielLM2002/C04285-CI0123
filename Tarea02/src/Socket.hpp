
#ifndef _SOCKET_
#define _SOCKET_

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
  /// ID of the socket
  int id;
  /// Number of the port
  int port;
  /// Check if ipv6 is going to be used
  bool ipv6;
  /// Struct of SSL
  void* SSLStruct;
  /// Context of SSL
  void* SSLContext;
  
 public:
  /// Constructor
  explicit Socket(char, bool = false);

  /// Constructor
  explicit Socket(int);

  /**
   * @brief Destroyer
   */
  ~Socket();


  int Connect(const char*, int);


  int Connect(const char*, const char*);

  int ConnectIPV6(const char*, int);


  int ConnectIPV6(const char*, const char*);
  

  void Close();


  int Read(char*, int);


  int Write(const char*);


  void InitSSL();

  void InitSSLContext();


  int SSLConnect(const char*, int);


  int SSLConnect(const char*, const char*);


  int SSLRead(const void*, int);


  int SSLWrite(const void*, int);
};

#endif  // !_SOCKET_
