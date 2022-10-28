#include "system.h"
#include "santa.h"

Santa::Santa() {
  sc = new Lock("santa");
  nombres.reserve(4);
  for (int i = 0; i < 4; i++) {
    elfs[i] = new Condition("elves");
    elfState[i] = Sleeping;
  }
  nombres.push_back('Grunon');
  nombres.push_back('Timido');
  nombres.push_back('Tom');
  nombres.push_back('Jerry');
}

Santa::~Santa() {
    delete sc;
    for (int i = 0; i < 4; i++) {
        delete elfs[i];
    }
}

int Santa::iniciarTrabajo(int elves) {
  int taller = -1;
  sc->Acquire();
  elfState[elves] = standBy;
  taller = casoNormal(nombres[elves]);
  if (elfState[elves] == standBy) {
    duendesDisponibles.push_back(elves);
    printf("El duende %ld esta esperando para poder trabajar en el taller \n", elves + 1);
    elfs[elves]->Wait(sc);
  }
  sc->Release();
  return taller;
}

void Santa::terminarTrabajo(int elves, int taller) {
  sc->Acquire();
  printf("el Duende %ld se fue a dormir \n", elves + 1);
  elfState[elves] = Sleeping;
  if (taller == 0) {
    taller1.remove(elves+1);
  } else { 
    taller2.remove(elves+1);
  }      
  for (std::list<int>::iterator it=duendesDisponibles.begin() ; it != duendesDisponibles.end(); ++it) {
    printf("revisando estado de duendes %ld que puedan trabajar \n", *it + 1);
    size_t size = duendesDisponibles.size();
    taller = casoNormal(nombres[*it]);
    if (size != duendesDisponibles.size()) {
      it = duendesDisponibles.begin();
    }
  }
  sc->Release();
}

bool Santa::tallerDisponible(int taller) {
  if (taller == 0) {
    return taller1.size() < 3;
  } else {
    return taller2.size() < 3;
  }
}

void Santa::eliminarDuende(int elf, int taller) {
  if (taller == 0) {
    taller1.remove(elf);
  } else {
    taller2.remove(elf);
  }
}

void Santa::stateDeTaller(int taller, bool state) {
  if (taller == 0) {
    taller1.clear();
  } else {
    taller2.clear();
  }
}

int Santa::casoNormal(char elves) {
  int taller = -1;
  if (elves == 'Grunon') {
    taller = casoSolitario();
  } else if (elves == 'Timido') {
    taller = casoTimido();
  } else if (elves == 'Tom') {
    taller = casoJerry();
  } else if (elves == 'Jerry') {
    taller = casoTom();
  }
  return taller;
}


int Santa::casoSolitario() {
  int taller = -1;
  if(this->taller1.empty()) {
    taller1.push_back(1);
    elfState[0] = working;
    duendesDisponibles.remove(0);
    taller = 0;
    elfs[0]->Signal(sc);
  } else if (this->taller2.empty()) {
    taller2.push_back(1);
    elfState[0] = working;
    duendesDisponibles.remove(0);
    taller = 1;
    elfs[0]->Signal(sc);
  }
  return taller;
}


int Santa::casoTom() {
  int taller = -1;
  if (taller1.size() <= 1 && *taller1.begin() != 1 && *taller1.begin() != 3) {
    taller1.push_back(4);
    elfState[3] = working;
    duendesDisponibles.remove(3);
    taller = 0;
    elfs[3]->Signal(sc);
  } else if (taller2.size() <= 1 && *taller2.begin() != 1 && *taller2.begin() != 3) {
    taller2.push_back(4);
    elfState[3] = working;
    duendesDisponibles.remove(3);
    taller = 1;
    elfs[3]->Signal(sc);
  }
  return taller;
}

int Santa::casoJerry() {
  int taller = -1;
  if (taller1.size() <= 1 && *taller1.begin() != 1 && *taller1.begin() != 4) {
    taller1.push_back(3);
    elfState[2] = working;
    duendesDisponibles.remove(2);
    taller = 0;
    elfs[2]->Signal(sc);
  } else if (taller2.size() <= 1 && *taller2.begin() != 1 && *taller2.begin() != 4) {
    taller2.push_back(3);
    elfState[2] = working;
    duendesDisponibles.remove(2);
    taller = 1;
    elfs[2]->Signal(sc);
  }
  return taller;
}

int Santa::casoTimido() {
  int taller = -1;
  if (taller1.size() == 1 && *taller1.begin() != 1){
    taller1.push_back(2);
    elfState[1] = working;
    duendesDisponibles.remove(1);
    taller = 0;
    elfs[1]->Signal(sc);
  } else if (taller2.size() == 1 && *taller2.begin() != 1) {
    taller2.push_back(2);
    elfState[1] = working;
    duendesDisponibles.remove(1);
    taller = 1;
    elfs[1]->Signal(sc);
  }
  return taller;
}

int Santa::casoEspecial() {
  int taller = -1;
  if (taller1.size() == 2 && taller2.size() == 2) {
    taller1.push_back(1);
    elfState[0] = working;
    duendesDisponibles.remove(0);
    taller = 0;
    elfs[0]->Signal(sc);
  } else if (taller1.size() == 2 && taller2.size() == 2) {
    taller2.push_back(1);
    elfState[0] = working;
    duendesDisponibles.remove(0);
    taller = 1;
    elfs[0]->Signal(sc);
  }
  return taller;
}



void Santa::print() {
  char elves;
  for (int i = 0; i < 4; ++i) {
    printf( "Duende %c: %s", elves, (elfState[i]==Sleeping) ? "Durmiendo\n" : (elfState[i]==standBy) ? "En espera\n" : "trabajando\n");
    if (elfState[i] == working) {
      printf(" en el taller #%d\n", buscarTaller(elves));
    }
  }
}

int Santa::buscarTaller(int elves) {
  int taller = -1;
  if (std::find(taller1.begin(), taller1.end(), elves) != taller1.end()) {
    taller = 0;
  } else if (std::find(taller2.begin(), taller2.end(), elves) != taller2.end()) {
    taller = 1;
  }
  return taller;
}
