#include "synch.h"
#include <algorithm>
#include <list>
#include <iterator>
#include <iostream>
class Santa {
public:
    std::list<char> tallerL;
    std::list<char> tallerH;
    std::list<char> sinAsignar;
    Santa();
    ~Santa();
    int inciarTrabajo(int duende );
    void terminarTrabajo(int duende, int taller);
    void print();
    void casoGrunon();
    void casoTimido();
    void casoJerry();
    void casoTom();
    bool tallerLleno(int taller);
    bool duendeTrabajando(int duende);
private:
    enum {Durmiendo, Esperando, Trabajando} estadoDuende[4];
    enum {Disponible, Ocupado} estadoTaller[2];
    Lock * dp;
    Condition * duendes[5];
    int taller[4];
    int duendesEnTaller;
};
