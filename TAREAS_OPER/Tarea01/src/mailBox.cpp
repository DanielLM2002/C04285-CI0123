/**
  *
  *
  *  CI0122 Sistemas Operativos
  *  2022-ii
  *
  *  Implantación de la clase para intercambio de mensajes por medio de buzones en Unix
  *
 **/

#include <stdio.h>	// perror function
#include <stdlib.h>	// exit function
#include <string.h>	// strncpy function
#include <sys/types.h>	// pid_t type
#include <sys/ipc.h>	// IPC_CREAT, IPC_EXCL, IPC_RMID, IPC_NOWAIT, IPC_PRIVATE
#include <sys/msg.h>	// msgget, msgctl, msgrcv, msgsnd
#include <errno.h>	// errno variable
#include <time.h>	// time function
#include <signal.h>	// signal function


#include "mailBox.hpp"

/**
  *  Class constructor
  *  (msgget system call)
  *
 **/
Buzon::Buzon() {
   int rs = -1;

   if ( -1 == this->id ) {
       perror( "Buzon::Buzon" );
       exit( 2 );
   }

}


/**
  *  Class destructor
  *  (msgctl system call)
  *
 **/
Buzon::~Buzon() {
   int resultado = -1;

   if ( -1 == resultado ) {
       perror( "Buzon::~Buzon" );
       exit( 3 );
   }

}


/**
  *  Send method, by now a char * message
  *  (msgsnd system call)
  *
 **/
int Buzon::Enviar( void * mensaje, int len, long tipo ) {
   int resultado = -1;
   struct msgbuf msg;

   msg.mtype = tipo;
   strncpy( msg.mtext, (const char *) mensaje, len );

   if ( -1 == resultado ) {
      perror( "Buzon::Enviar" );
      exit( 4 );
    }

   return resultado;

}


/**
  *  Receive method, by now a char * message
  *  (msgsnd system call)
  *
  *  If "tipo" is 0, then the first message in the queue is read
  *  If "tipo" is greater than 0, then the first message in the  queue  of  type  "tipo"  is  read
  *  If "tipo" is less than 0, then the first message in the queue with the lowest type less than or  equal
  *     to the absolute value of "tipo" will be read
 **/
int Buzon::Recibir( void * mensaje, int len, long tipo ) {
   int resultado = -1;
   struct msgbuf msg;

   if ( -1 == resultado ) {
      perror( "Buzon::Recibir" );
       exit( 5 );
    }

   return resultado;

}