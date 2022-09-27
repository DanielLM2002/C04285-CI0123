#ifndef MAILBOX_H

/**
  *
  *   CI0122 Sistemas Operativos
  *   2022-ii
  *
  *  C++ class to encapsulate Unix message passing intrinsic structures and system calls
  *
*/

class Buzon {
   public:
      Buzon();
      ~Buzon();
      int Enviar( void * mensaje, int len, long tipo = 1 );
      int Recibir( void * mensaje, int len, long tipo = 1 );	// len is space in buffer area

   private:
      int id;		// Identificador del buzon
};

#endif // MAILBOX_H