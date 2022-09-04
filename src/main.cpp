
#define DEFAULT
#define SIZE 1024

#include <stdio.h>
#include <string.h>

#include "Socket.hpp"
// #include "DataSource.hpp"
#include "Round.hpp"
//#include "JsonDataSource.hpp"

int main(int argc, char * argv[]) {
  const char* os = "http://os.ecci.ucr.ac.cr/";
  //const char* osn = "10.1.104.187";  // Public ip
  const char* osn = "163.178.104.187";  // Private ip  
  const char* request = "GET /futbol/2018/world-cup-2018.json HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";
  Socket s('s');
  char a[512];
  s.Connect(osn, 80);
  s.Write(request);
  while (s.Read(a, 512) != 0) {
    printf("%s\n", a);
    memset(a, 0, 512);
  }

  //char * os = (char *) "http://os.ecci.ucr.ac.cr/";
  //char * reqOS = (char *) "GET /futbol/2018/world-cup-2018.json HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";
  //Socket s( 's', false );	// Create a new stream socket for IPv4
  //char a[ SIZE ];

  //memset( a, 0, SIZE );
  //s.InitSSL();
  //s.SSLConnect( os, (char *) "https" );
  //s.SSLWrite(  (void *) reqOS, strlen( reqOS ) );
  //while ( (s.SSLRead( a, SIZE )) > 0 ) {   // Waits until conection close
  //  printf( "%s\n", a );
  //  memset( a, 0, SIZE );
  //}
  //printf( "%s\n", a );
}

