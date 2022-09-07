
/*
 *  Ejemplo de sockets con SSL
 *  Compilar con:
 *     g++ -g Socket.cc ssl-ipv4-test.cc -lssl -lcrypto -o ssl-ipv4-test.out
 *
 */

#include <stdio.h>
#include <string.h>

#include "Socket.h"

#define SIZE 1024

int main( int argc, char * argv[] ) {
   char * os = (char *) "os.ecci.ucr.ac.cr";
   char * WM = (char *) "10.1.104.187";
   // char * WM = (char *) "163.178.184.187";
   char * reqOS = (char *) "GET /futbol/2018/index.php?country=FRA HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";
   Socket s( 's', false );	// Create a new stream socket for IPv4
   char a[ SIZE ];
   int count;

   memset( a, 0, SIZE );
   s.InitSSL();
   s.SSLConnect( WM, 443);
   s.SSLWrite(  (void *) reqOS, strlen( reqOS ) );
   while (  (count = s.SSLRead( a, SIZE )) > 0 ) {
      printf( "%s\n", a );
      memset( a, 0, SIZE );
   }

   printf( "%s\n", a );

}