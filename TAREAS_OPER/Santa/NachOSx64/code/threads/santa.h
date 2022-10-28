#include "synch.h"
#include <algorithm>
#include <list>
#include <iterator>
#include <iostream>
class Santa {
public:
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
private:
    enum {Durmiendo, Esperando, Trabajando} estadoDuende[4];
    enum {Disponible, Ocupado} estadoTaller[2];
    Lock * dp;
    Condition * duendes;
    int taller[4];
    int duendesEnTaller;
    std::list<char> tallerL;
    std::list<char> tallerH;
    std::list<char> sinAsignar;
};
