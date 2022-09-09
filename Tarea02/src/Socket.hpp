#ifndef Socket_hpp
#define Socket_hpp

class Socket{

   public:
      // Otros metodos definidos por usted

// SSL methods
   private:
      void InitSSLContext();
   public:
      void InitSSL();
      void SSLConnect( char *, int );
      void SSLConnect( char *, char * );
      int SSLRead( void *, int );
      int SSLWrite( const void *, int );

   private:
      // Otras instancias de la clase definidas por usted

// SSL instance variables
      void * SSLContext;		// SSL context
      void * SSLStruct;		// SSL BIO

};

#endif

