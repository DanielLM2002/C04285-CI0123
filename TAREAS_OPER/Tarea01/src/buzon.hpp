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


class Buzon {
 private:
    int id;
 public:
      Buzon();
      ~Buzon();
      int enviar(void* mensaje,int len, long mid = 1);
      int recibir(void* mensaje,int len, long mid = 1);
};
#endif // BUZON_H
