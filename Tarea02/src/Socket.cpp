
#include "Socket.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

/* 
   char tipo: el tipo de socket que quiere definir
      's' para "stream
      'd' para "datagram"   if(!ipv6){
   bool ipv6: si queremos un socket para IPv6
 */
Socket::Socket( char type, bool ipv6 ){
   this->ipv6 = ipv6;
   size_t socketType;
   size_t domain;

   socketType = (type == 's') ? SOCK_STREAM : SOCK_DGRAM;
   domain = this->ipv6 ? AF_INET6 : AF_INET;

   idSocket = socket(domain, socketType, 0);

   // SSL
   this->SSLContext = NULL;
   this->SSLStruct = NULL;
}


Socket::~Socket(){
   Close();

   //SSL 
   if (this->SSLContext) {
      SSL_CTX_free( (SSL_CTX *) this->SSLContext );
   }
   if (this->SSLStruct) {
      SSL_free( (SSL *) this->SSLStruct );
   }
}

void Socket::Close(){
   close(this->idSocket);
}

int Socket::Connect( const char * hostip, int port ) {
   struct sockaddr_in host4;

   memset( (char *) &host4, 0, sizeof( host4 ) );
   host4.sin_family = AF_INET;
   inet_pton( AF_INET, hostip, &host4.sin_addr );
   host4.sin_port = htons( port );
   int st = connect( idSocket, (sockaddr *) &host4, sizeof( host4 ) );
   if ( -1 == st ) {
      perror( "Socket::Connect" );
      exit( 2 );
   }
   return st;
}


void Socket::InitSSLContext() {
   const SSL_METHOD * method = TLS_client_method();
   SSL_CTX * context = SSL_CTX_new(method);
   if (!context) {
      perror( "Socket::InitSSLContext" );
      exit( 23 );
   }
   this->SSLContext = (void *) context;
}

void Socket::InitSSL() {
   this->InitSSLContext();
   SSL * ssl = SSL_new( ((SSL_CTX *) this->SSLContext ) );
   if (!ssl) {
      perror( "Socket::InitSSL" );
      exit( 23 );
   }
   this->SSLStruct = (void *) ssl;
}

int Socket::SSLConnect( char * host, int port ) {
   int st;
   this->Connect( host, port );
   SSL_set_fd( (SSL *) this->SSLStruct, this->idSocket );
   st = SSL_connect( (SSL *) this->SSLStruct );
   if ( -1 == st ) {
      perror( "Socket::SSLConnect" );
      exit( 23 );
   }
   return st;
}

int Socket::SSLConnect( char * host, char * service){
   return -1;
}

int Socket::SSLRead( void * buffer, int size){
   int st;
   st = SSL_read( (SSL *) this->SSLStruct, buffer, size );
   if ( -1 == st ) {
      perror( "Socket::SSLRead" );
      exit( 23 );
   }
   return st;
}

int Socket::SSLWrite( void * buffer, int size){
   int st;
   st = SSL_write( (SSL *) this->SSLStruct, buffer, size );
   if ( -1 == st ) {
      perror( "Socket::SSLWrite" );
      exit( 23 );
   }
   return st;
}
