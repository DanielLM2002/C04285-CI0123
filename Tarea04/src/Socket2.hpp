

#ifndef Socket_h
#define Socket_h

class Socket{

    public:
        Socket( char, bool = false );
        Socket( int );
        ~Socket();
        int Connect( char *, int );
        int Connect( char *, char * );
        void Close();
        int Read( char *, int );
        int Write( char *, int );
        int Write( char * );
        int Listen( int );
        int Bind( int );
        Socket * Accept();
        int Shutdown( int );
        void SetIDSocket( int );
        int sendTo( const void *, int, void * );
        int recvFrom( void *, int, void * );

    private:
        int idSocket;
        bool ipv6;
        int port;
};

#endif