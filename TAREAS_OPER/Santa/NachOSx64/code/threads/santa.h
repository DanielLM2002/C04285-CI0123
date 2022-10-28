#include "synch.h"
#include <list>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

class Santa {
 private:
  std::vector<char> nombres;
  std::list<char> taller1;
  std::list<char> taller2;
  std::list<int> duendesDisponibles;
  Lock* sc;
  Condition* elfs[5];
  
  enum { Sleeping, standBy, working } elfState[4];

 public:
  Santa();
  ~Santa();
  int iniciarTrabajo(int);
  void terminarTrabajo(int, int);
  void print();
  bool tallerDisponible(int);
  bool elfoDisponible(int);
  void stateDeTaller(int, bool);
  bool revisarDuende(char, int);
  void eliminarDuende(int, int);
  int buscarTaller(int);

 private:
  int casoNormal(char); 
  int casoSolitario();
  int casoTimido();
  int casoJerry();
  int casoTom();
  int casoEspecial();
};