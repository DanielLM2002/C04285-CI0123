#include <stdio.h>	// printf function
#include <unistd.h>	// _exit function
#include <sys/wait.h>	// wait function
#include <stdlib.h>	// exit function
#include <string.h>	// strlen function
#include <sys/types.h>	// pid_t type
#include <sys/ipc.h>	// IPC_CREAT, IPC_EXCL, IPC_RMID, IPC_NOWAIT, IPC_PRIVATE
#include <sys/msg.h>	// msgget, msgctl, msgrcv, msgsnd
#include <errno.h>	// errno variable
#include <time.h>	// time function
#include <signal.h>	// signal function
#include <sys/shm.h>	// shmget, shmctl, shmat, shmdt
#include <sys/sem.h>	// semget, semctl, semop

int participantes = 10;		// Valor predefinido para la cantidad de participantes

struct msgbuf1 {			// Estructura ejemplo para el intercambio de mensajes
   long mtype;
   int papa;
   int participantes;
};
 

/**
  *  Aplica las reglas de Collatz al valor de la papa
  *
  *  Se simula el estallido de la papa cuando el valor retornado por esta función es uno
 **/
int cambiarPapa( int papa ) {

   if ( 1 == (papa & 0x1) ) {			// papa es impar
            papa = (papa << 1) + papa + 1;	// papa = papa * 2 + papa + 1
         } else {
            papa >>= 1;				// n = n / 2, utiliza corrimiento a la derecha, una posicion
         }

   return papa;

}


/**
 *   Procedimiento para simular una persona participante en la ronda
 *
 *   Recibe la identificación del buzón y la identificación de la persona
 *
 **/
int persona( int buzon, int id ) {

   _exit( 0 );	// Everything OK

}

int main( int argc, char ** argv ) {
   int buzon, id, i, j, st;
   struct msgbuf1 m;

   if ( argc > 1 ) {
      participantes = atoi( argv[ 1 ] );
   }
   if ( participantes <= 0 ) {
      participantes = 10;
   }

   srandom( time( 0 ) );

   printf( "Creando una ronda de %d participantes\n", participantes );
   for ( i = 1; i <= participantes; i++ ) {
      if ( ! fork() ) {
         persona( buzon, i * 10 );	// Este proceso simula una persona participante
      }
   }


   do {		// Escoge el proceso que comienza el juego, definiendo el tipo de mensaje
      m.mtype = (random() % participantes) * 10;
   } while ( m.mtype == 0 );
   m.papa = random() % 30;	// Define el valor inicial de la papa
   m.participantes = participantes;
   printf( "Receptor del primer mensaje %d, valor de la papa %d\n", m.mtype, m.papa );

   for ( i = 1; i <= participantes; i++ ) {
      j = wait( &st );		// Esperar hasta que finalicen los procesos
   }

   printf( "Main: valor de la papa %d\n", m.papa );

// Elimina los recursos compartidos

}
