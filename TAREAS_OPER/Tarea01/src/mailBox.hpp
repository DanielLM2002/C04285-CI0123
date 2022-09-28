#ifndef MAILBOX_H

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