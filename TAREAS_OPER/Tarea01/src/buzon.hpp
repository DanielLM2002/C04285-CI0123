#ifndef BUZON_H
#define BUZON_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

// Constante que almacena el de carné.
#define KEY2 0xA33044

#define MSG_SIZE 1024
// Clase encargada de compartir mensajes entre procesos.
class Buzon{
public:
/**
 * @brief Constructor por defecto
 **/
   Buzon();
/**
 * @brief Destructor
 **/
   ~Buzon();
/**
 * @brief Metodo que se encarga de Enviar el mensaje 
 * @param mensaje El mensaje a Enviar en un buffer
 * @param mid El Id del Buzon
 **/
   void enviar(void* mensaje, long mid);
/**
 * @brief Metodo que se encarga de Recibir el mensaje 
 * @param mensaje El mensaje a Recibir en un buffer
 * @param mid El Id del Buzon
 **/
   void recibir(void* mensaje, int len, long mid = 1);

protected:
   /// El id del Buzon en memoria
   int id;
/**
 * @brief Estructura que almacena el contenido de los mensajes.
 * @param mesg_type El tipo de mensaje
 * @param mesg_text El buffer del mensaje
 **/   
   struct msgbuf{
      //Ver este enlace para rellenar: https://tldp.org/LDP/lpg/node30.html
       long mesg_type;
	    char mesg_text[MSG_SIZE];
   } message;
};

#endif // BUZON_H
