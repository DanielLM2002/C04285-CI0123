#include "synch.h"

class Santa {

public:
    Santa();
    ~Santa();
    void iniciarTrabajo( long who );
    void terminarTrabajo( long who );
    bool estaTrabajando();
    void iniciarTaller(); 
    void despertar();
    void print();

private:
    void test( long who );
    enum { Sleeping, Working, Waiting } state[ 4 ];
    Lock * sc;			// Locks for each method in class
    Condition * self[ 4 ];

};
