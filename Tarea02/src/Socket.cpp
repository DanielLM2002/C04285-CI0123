
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


// Otros includes

// SSL includes
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "Socket.h"

Socket::Socket( char t, bool ipv6 ){

// Otra codigo de su constructor

// SSL initialization
   this->SSLContext = nullptr;
   this->SSLStruct = nullptr;

}


Socket::Socket( int id ) {

   idSocket = id;

}


Socket::~Socket() {

   Close();

// SSL destroy
   if ( nullptr != this->SSLContext ) {
      SSL_CTX_free( (SSL_CTX *) this->SSLContext );
   }
   if ( nullptr != this->SSLStruct ) {
      SSL_free( (SSL *) this->SSLStruct );
   }

}


void Socket::Close(){

   close( this->idSocket );

}



// SSL methods

/*
 *
 */
void Socket::InitSSLContext() {
   const SSL_METHOD * method = TLS_client_method();
   SSL_CTX * context = SSL_CTX_new( method );

   if ( nullptr == context ) {
        perror( "Socket::InitSSLContext" );
        exit( 23 );
    }

    this->SSLContext = (void *) context;

}


/*
 *
 */
void Socket::InitSSL() {

   this->InitSSLContext();
   SSL * ssl = SSL_new( ((SSL_CTX *) this->SSLContext ) );
   if ( nullptr == ssl ) {
      perror( "Socket::InitSSL" );
      exit( 23 );
   }
   this->SSLStruct = (void *) ssl;

}


/*
 *
 */
void Socket::SSLConnect( char * host, int port ) {
   int resultado;

   this->Connect( host, port );	// Establish a non ssl connection first
   SSL_set_fd( (SSL *) this->SSLStruct, this->idSocket );
   resultado = SSL_connect( (SSL *) this->SSLStruct );
   if ( -1 == resultado ) {
      perror( "Socket::SSLConnect" );
      exit( 23 );
   }

}


/*
 *
 */
void Socket::SSLConnect( char * host, char * service ) {
   int resultado;

   this->Connect( host, service );
   SSL_set_fd( (SSL *) this->SSLStruct, this->idSocket );
   resultado = SSL_connect( (SSL *) this->SSLStruct );
   if ( -1 == resultado ) {
      perror( "Socket::SSLConnect" );
      exit( 23 );
   }


}


/*
 *
 */
int Socket::SSLRead( void * buffer, int size ) {
   int resultado;

   resultado = SSL_read( (SSL *) this->SSLStruct, buffer, size );
   if ( -1 == resultado ) {
      perror( "Socket::SSLRead" );
      exit( 23 );
   }

   return resultado;

}


/*
 *
 */
int Socket::SSLWrite( const void *buffer, int size ) {
   int resultado;

   resultado = SSL_write( (SSL *) this->SSLStruct, buffer, size );
   if ( -1 == resultado ) {
      perror( "Socket::SSLWrite" );
      exit( 23 );
   }

   return resultado;

}