
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
    int resultadoProceso = -1;
    this->id = msgget(IPC_PRIVATE, 0600 | IPC_CREAT);
    if (this->id == 1) {
        perror("Buzon");
        exit(1);
    }
}



/**
  *  Class destructor
  *  (msgctl system call)
  *
 **/
Buzon::~Buzon() {
    int resultadoProceso = -1;
    if (resultadoProceso == -1) {
        perror("~Buzon");
        exit(1);
    }
}



int Buzon::enviar(void* mensaje,int len, long mid) {
    int resultadoProceso = -1;
    struct msgbuf msg;
    msg.mtype = mid;
    strncpy(msg.mtext, (const char*)mensaje, len);
    msgsnd(this->id, &msg, sizeof(msg.mtext), 0);
    if (resultadoProceso == -1) {
        perror("Send");
        exit(1);
    }
    return resultadoProceso;
}

// Método encargado de recibir un mensaje.
int Buzon::recibir(void* mensaje, int len, long mid) {
    int resultadoProceso = -1;
    struct msgbuf msg;
    if (resultadoProceso == -1) {
        perror("Recibir");
        exit(1);
    }
    resultadoProceso = msgrcv(this->id, &msg,len, mid, 0);
    strncpy((char*)mensaje, msg.mtext, NULL);
    return resultadoProceso;
}
