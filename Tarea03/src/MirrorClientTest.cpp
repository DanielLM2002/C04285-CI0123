/*
 *
 */
#include <stdio.h>
#include "Socket.hpp"


int main( int argc, char ** argv ) {
   Socket s;     // Crea un socket de IPv4, tipo "stream"
   char buffer[ 512 ];

   s.Connect( "ip address in dot decimal format", 9876 ); // Same port as server
   s.Write( argv[1] );		// Send first program argument to server
   s.Read( buffer, 512 );	// Read the answer sent back from server
   printf( "%s", buffer );	// Print the received string

}