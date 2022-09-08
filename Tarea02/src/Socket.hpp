/*
 *  Establece la definicion de la clase Socket para efectuar la comunicacion
 *  de procesos que no comparten memoria, utilizando un esquema de memoria
 *  distribuida.  El desarrollo de la clase se hara en dos etapas, primero
 *  los metodos necesarios para los clientes, en la otra etapa los otros
 *  metodos para los servidores
 */

#ifndef Socket_h
#define Socket_h

class Socket{

    public:
        Socket( char, bool = false );
        Socket( int );
        ~Socket();
        void Close();
        int Connect( const char *, int );
        void InitSSLContext();
        void InitSSL();
        int SSLConnect( char *, int );
        int SSLConnect( char *, char * );
        int SSLRead( void *, int );
        int SSLWrite( void *, int );
        
    private:
        int idSocket;
        int port;
        bool ipv6;
        void * SSLContext;	// SSL context
	    void * SSLStruct;	// SSL BIO
};

#endif

