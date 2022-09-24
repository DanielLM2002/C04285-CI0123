/// @copyright 2022 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3

#define SIZE 1024
//#define SERVER
#define CLIENT

#include "Socket.hpp"

#ifdef SERVER
int main() {
  int childpid;
  char a[512];
  Socket s1('s', false), *s2;

  s1.Bind(9876);		// Port to access this mirror server
  s1.Listen(5);		// Set backlog queue to 5 conections

  for( ; ; ) {
    s2 = s1.Accept();	 	// Wait for a conection
    childpid = fork();	// Create a child to serve the request
    if (childpid < 0)
      perror("server: fork error");
    else if (0 == childpid) {  // child code
      s1.Close();	// Close original socket in child
      s2->Read(a, 512); // Read a string from client
      s2->Write(a);	// Write it back to client, this is the mirror function
      exit(0);	// Exit
    }
    s2->Close();		// Close socket in parent
   }
}
#endif // !SERVER

#ifdef CLIENT
int main(int argc, char ** argv) {
  Socket s('s', false);     // Crea un socket de IPv4, tipo "stream"
  char buffer[512];
  s.Connect("127.0.0.1", 9876); // Same port as server
  s.Write(argv[1]);		// Send first program argument to server
  s.Read(buffer, 512);	// Read the answer sent back from server
  printf("%s \n", buffer);	// Print the received string
}
#endif // !CLIENT