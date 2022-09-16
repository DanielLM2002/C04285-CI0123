#define SIZE 1024

#include "Socket.hpp"

int main(int argc, char * argv[]) {
  char* os = (char*) "http://os.ecci.ucr.ac.cr/";
  const char* osn = "163.178.104.187";  // Private ip 
  const char* request = "GET /futbol/2018/clasificacion.txt HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";
  Socket s('s', false);	// Create a new stream socket for IPv4
  char a[SIZE];
  memset(a, 0, SIZE);
  s.InitSSL();
  s.SSLConnect(osn, 443);
  s.SSLWrite((void*) request, strlen(request));
  while ((s.SSLRead(a, SIZE)) > 0) {   // Waits until conection close
   printf("%s\n", a);
   memset(a, 0, SIZE);
  }
  printf("%s\n", a);
}
