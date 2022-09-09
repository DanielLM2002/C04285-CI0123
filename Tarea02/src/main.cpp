/*
 *  Ejemplo de sockets con SSL
 *  Compilar con:
 *     g++ -g Socket.cc ssl-ipv4-test.cc -lssl -lcrypto -o ssl-ipv4-test.out
 *
 */

#include <stdio.h>
#include <string.h>
//#include "ssl.h"
#include "Socket.hpp"
#include <iostream>


#define SIZE 1024

int main( int argc, char * argv[] ) {
   char * dirIp = (char *) "163.178.104.187";
   char * os = (char *) "os.ecci.ucr.ac.cr";
   char * reqOS = (char *) "GET /futbol/2018/clasificacion.txt HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";
   //https://raw.githubusercontent.com/openfootball/worldcup.json/master/2018/worldcup.standings.json
   Socket s( 's', false ); // Create a new stream socket for IPv4
   char a[ SIZE ];
   std::string carry = "";

   memset( a, 0, SIZE );
   s.InitSSL();
   s.SSLConnect(dirIp, (char *) "https");
   s.SSLWrite(  (void *) reqOS, strlen( reqOS ) );
   while (  (s.SSLRead( a, SIZE )) > 0 ) {
      //printf( "%s\n", a );
      carry += a;
      memset( a, 0, SIZE );
   }

   std::cout << carry << std::endl;
   //printf( "%s\n", a );

}