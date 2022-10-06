
#include <stdio.h>	// perror function
#include <stdlib.h>	// exit function
#include <string.h>	// strncpy function
#include <sys/types.h>	// pid_t type
#include <sys/ipc.h>	// IPC_CREAT, IPC_EXCL, IPC_RMID, IPC_NOWAIT, IPC_PRIVATE
#include <sys/msg.h>	// msgget, msgctl, msgrcv, msgsnd
#include <errno.h>	// errno variable
#include <time.h>	// time function
#include <signal.h>	// signal function


#include "buzon.hpp"

/**
  *  Class constructor
  *  (msgget system call)
  *
 **/
Buzon::Buzon()
{
     // Crear el Buzón con msgget
    this->id = msgget(KEY2, 0666 | IPC_CREAT);
   
}



/**
  *  Class destructor
  *  (msgctl system call)
  *
 **/
Buzon::~Buzon()
{
    struct msqid_ds str;
    // Eliminar el buzón con msgctl
    	if (msgctl(id, IPC_RMID, &str) == -1) {
		fprintf(stderr, "No se puede eliminar el Buzon.\n");
		exit(EXIT_FAILURE);
	}
    (void)str;
}



void Buzon::enviar(void* mensaje, long mid)
{
    // Inicializar el contenido del mensaje con msgbuf
    message.mesg_type = mid;
	strcpy(message.mesg_text, (char*)mensaje);
    // Enviar el mensaje con msgsnd
    msgsnd(this->id, &message, sizeof(message.mesg_text), 0);
}

// Método encargado de recibir un mensaje.
void Buzon::recibir(void* mensaje, int len, long mid)
{
    // Inicializar el contenido del mensaje con msgbuf
    /// message.mesg_type = mid;

    // Recibir el mensaje con msgrcv
    msgrcv(this->id, &message, len, mid, 0);
    // Sacar el mensaje del buzón. Sugerencia: strcpy
    strcpy((char*)mensaje,message.mesg_text);
}
